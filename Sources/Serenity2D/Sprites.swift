import Serenity

public final class Spritesheet<Texture>: Component {
    public let image: String; public let imageSize: Vec2<Float>; public var texture: Texture?
    private let loadTexture: (String) throws -> Texture
    public init(_ parent: Entity, image: String, imageSize: Vec2<Float>, loadTexture: @escaping (String) throws -> Texture) {
        self.image = image; self.imageSize = imageSize; self.loadTexture = loadTexture; super.init(parent)
    }
    public override func initialize(_ context: Node) { texture = try? loadTexture(image) }
    public func load(_ entity: Entity, position: (x: Int, y: Int) = (0, 0)) -> Sprite<Texture> {
        Sprite(entity, sheet: self, position: Vec2(Float(position.x) * imageSize.x, Float(position.y) * imageSize.y), size: imageSize)
    }
}

public final class Sprite<Texture>: Component {
    private weak var sheet: Spritesheet<Texture>?; private let source: Vec2<Float>; public let size: Vec2<Float>
    fileprivate init(_ parent: Entity, sheet: Spritesheet<Texture>, position: Vec2<Float>, size: Vec2<Float>) {
        self.sheet = sheet; self.source = position; self.size = size; super.init(parent)
    }
    public override func update(_ context: Node) {
        guard let renderer = context as? Renderer, let transform = component(Transform.self),
              let sheet, let texture = sheet.texture, let painter = renderer.first(Painter<Texture>.self) else { return }
        painter.draw(texture, source: Vec4(position: source, size: size), destination: Vec4(position: transform.screenPosition(), size: size))
    }
}

public final class Painter<Texture>: Node {
    private let drawTexture: (Texture, Vec4<Float>?, Vec4<Float>) -> Void
    public init(_ parent: Node, draw: @escaping (Texture, Vec4<Float>?, Vec4<Float>) -> Void) { self.drawTexture = draw; super.init(parent) }
    public func draw(_ texture: Texture, source: Vec4<Float>?, destination: Vec4<Float>) { drawTexture(texture, source, destination) }
}
