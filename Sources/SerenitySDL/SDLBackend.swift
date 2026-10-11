import CSDL3
import Foundation
import Serenity

public typealias SDLTexture = UnsafeMutablePointer<SDL_Texture>

public enum SDLError: Error, CustomStringConvertible {
    case initializationFailed(String)
    case operationFailed(String)

    public var description: String {
        switch self {
        case let .initializationFailed(message): "SDL initialization failed: \(message)"
        case let .operationFailed(message): "SDL operation failed: \(message)"
        }
    }
}

public final class SDLBackend: RendererBackend {
    private var window: OpaquePointer?
    private var renderer: OpaquePointer?
    private var initialized = false
    private var textures: [SDLTexture] = []

    public init() {}

    deinit {
        shutdown()
    }

    public func createWindow(title: String, width: Int, height: Int) throws {
        guard !initialized else { return }

        guard SDL_Init(SDL_INIT_VIDEO) else {
            throw SDLError.initializationFailed(lastError)
        }
        initialized = true

        let createdWindow = title.withCString {
            SDL_CreateWindow($0, Int32(width), Int32(height), 0)
        }
        guard let createdWindow else {
            shutdown()
            throw SDLError.operationFailed(lastError)
        }
        window = createdWindow

        guard let createdRenderer = SDL_CreateRenderer(createdWindow, nil) else {
            shutdown()
            throw SDLError.operationFailed(lastError)
        }
        renderer = createdRenderer

        _ = SDL_SetRenderVSync(createdRenderer, 1)
    }

    public func clear() {
        guard let renderer else { return }
        guard SDL_RenderClear(renderer) else {
            reportFailure("SDL_RenderClear")
            return
        }
    }

    public func present() {
        guard let renderer else { return }
        guard SDL_RenderPresent(renderer) else {
            reportFailure("SDL_RenderPresent")
            return
        }
    }

    public func pollQuit() -> Bool {
        var event = SDL_Event()
        while SDL_PollEvent(&event) {
            if event.type == UInt32(SDL_EVENT_QUIT.rawValue) {
                return true
            }
        }
        return false
    }

    public func loadBMP(_ path: String) throws -> SDLTexture {
        guard let renderer else {
            throw SDLError.operationFailed("load texture before creating the renderer")
        }
        let surface = path.withCString { SDL_LoadBMP($0) }
        guard let surface else {
            throw SDLError.operationFailed(lastError)
        }
        defer { SDL_DestroySurface(surface) }
        guard let texture = SDL_CreateTextureFromSurface(renderer, surface) else {
            throw SDLError.operationFailed(lastError)
        }
        textures.append(texture)
        return texture
    }

    public func destroyTexture(_ texture: SDLTexture) {
        SDL_DestroyTexture(texture)
        textures.removeAll { $0 == texture }
    }

    public func draw(
        _ texture: SDLTexture,
        source: Vec4<Float>?,
        destination: Vec4<Float>
    ) {
        guard let renderer else { return }
        var destinationRect = SDL_FRect(x: destination.x, y: destination.y, w: destination.width, h: destination.height)
        let rendered: Bool
        if var sourceRect = source.map({ SDL_FRect(x: $0.x, y: $0.y, w: $0.width, h: $0.height) }) {
            rendered = withUnsafePointer(to: &sourceRect) {
                SDL_RenderTexture(renderer, texture, $0, &destinationRect)
            }
        } else {
            rendered = SDL_RenderTexture(renderer, texture, nil, &destinationRect)
        }
        guard rendered else {
            reportFailure("SDL_RenderTexture")
            return
        }
    }

    public func shutdown() {
        textures.forEach { SDL_DestroyTexture($0) }
        textures.removeAll()
        if let renderer {
            SDL_DestroyRenderer(renderer)
            self.renderer = nil
        }
        if let window {
            SDL_DestroyWindow(window)
            self.window = nil
        }
        if initialized {
            SDL_Quit()
            initialized = false
        }
    }

    private var lastError: String {
        String(cString: SDL_GetError())
    }

    private func reportFailure(_ operation: String) {
        let message = "\(operation) failed: \(lastError)"
        print(message)
    }
}
