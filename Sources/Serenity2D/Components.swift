import Serenity

public final class Transform: Component {
    private(set) public var position: Vec2<Float>
    public var delta: Vec2<Float> = .zero
    private var history: [Vec2<Float>] = []
    public init(_ parent: Entity, position: Vec2<Float> = .zero) { self.position = position; super.init(parent) }
    public func setPosition(_ value: Vec2<Float>) {
        history.append(position); if history.count > 20 { history.removeFirst() }; position = value
    }
    public func translate(_ value: Vec2<Float>) { setPosition(position + value) }
    public func translate(x: Float, y: Float) { translate(Vec2(x, y)) }
    public func rollback() { guard let value = history.popLast() else { return }; position = value }
    public func screenPosition(for camera: Camera? = nil) -> Vec2<Float> {
        guard let camera, let transform = camera.first(Transform.self) else { return position }
        return position - transform.position
    }
    public override func update(_ context: Node) { translate(delta) }
}

public final class Camera: Entity {
    private let screenSize: Vec2<Float>
    private weak var following: Transform?
    private var lerpFactor: Float = 0
    public init(_ parent: Scene, screenSize: Vec2<Float>, name: String? = nil) {
        self.screenSize = screenSize; super.init(parent, name: name)
        _ = Transform(self, position: -screenSize / 2)
    }
    public func follow(_ transform: Transform, lerpFactor: Float) { following = transform; self.lerpFactor = lerpFactor }
    public override func updateComponents(_ context: Node) {
        if let following, let transform = first(Transform.self) {
            transform.setPosition(lerp(transform.position, following.position - screenSize / 2, lerpFactor))
        }
        super.updateComponents(context)
    }
}

public final class CollisionBox: Component {
    public enum Mode { case solid, movable, passThrough, doNotAvoid }
    public let mode: Mode
    public var size: Vec2<Float>?
    public init(_ parent: Entity, mode: Mode, size: Vec2<Float>? = nil) { self.mode = mode; self.size = size; super.init(parent) }
    public var rect: Vec4<Float> {
        guard let transform = component(Transform.self), let size else { return .init() }
        return Vec4(position: transform.position, size: size)
    }
    public func overlaps(_ other: CollisionBox) -> Bool { Collision.overlaps(rect, other.rect) }
    public func overlaps(_ point: Vec2<Float>) -> Bool { Collision.overlaps(rect, Vec4(position: point, size: .zero)) }
    public func hasCollision() -> Bool {
        guard let game else { return false }
        return game.descendants(CollisionBox.self).contains { $0 !== self && $0.mode != .passThrough && overlaps($0) }
    }
    public override func update(_ context: Node) {
        guard mode == .movable, let transform = component(Transform.self), let game else { return }
        for other in game.descendants(CollisionBox.self) where other !== self && other.mode != .passThrough && other.mode != .doNotAvoid {
            while overlaps(other) { transform.rollback() }
        }
    }
}

public final class PlatformerPhysics: Component {
    public var fallSpeed: Float; public var jumpSpeed: Float
    public init(_ parent: Entity, fallSpeed: Float = 0.05, jumpSpeed: Float = 3) {
        self.fallSpeed = fallSpeed; self.jumpSpeed = jumpSpeed; super.init(parent)
    }
    public override func update(_ context: Node) {
        guard let box = component(CollisionBox.self), let transform = component(Transform.self) else { return }
        transform.delta.y = box.hasCollision() ? 0 : transform.delta.y + fallSpeed
    }
    public func jump() { component(Transform.self)?.delta.y -= jumpSpeed }
}
