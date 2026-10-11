import Foundation

public protocol RendererBackend: AnyObject {
    func createWindow(title: String, width: Int, height: Int) throws
    func clear(); func present(); func pollQuit() -> Bool
}

public final class Renderer: Node {
    public let title: String; public let width: Int; public let height: Int
    public let backend: RendererBackend?
    public private(set) var isRunning = false
    public init(_ parent: Node, title: String, width: Int = 640, height: Int = 480, backend: RendererBackend? = nil) {
        self.title = title; self.width = width; self.height = height; self.backend = backend; super.init(parent)
    }
    public func run() throws {
        try backend?.createWindow(title: title, width: width, height: height)
        guard let game = (parent as? Game) ?? parent?.first(Game.self) else { return }
        game.entities.forEach { $0.initializeComponents(self) }; isRunning = true
        while isRunning && !(backend?.pollQuit() ?? true) {
            backend?.clear(); game.entities.forEach { $0.updateComponents(self) }; backend?.present()
        }
    }
    public func quit() { isRunning = false }
}

public final class TimerSystem: Node {
    public private(set) var delta: TimeInterval = 1.0 / 60.0
    private var running = false
    public init(_ parent: Node) { super.init(parent) }
    public func start(hz: Int = 60, tick: ((TimeInterval) -> Void)? = nil) {
        guard !running else { return }; running = true; let interval = 1.0 / Double(hz); var last = Date()
        while running { let now = Date(); delta = now.timeIntervalSince(last); last = now; tick?(delta); Thread.sleep(forTimeInterval: max(0, interval - delta)) }
    }
    public func stop() { running = false }
}

public enum Keyboard {
    nonisolated(unsafe) public static var pressed: Set<Int> = []
    public static func isPressed(_ keyCode: Int) -> Bool { pressed.contains(keyCode) }
}
