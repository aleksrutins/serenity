import Serenity
import Serenity2D
import SerenitySDL
import Foundation

private let width: Float = 800
private let height: Float = 600
private let tileSize: Float = 32

private final class PlayerController: Component {
    private var speed: Float = 4
    private var physics: PlatformerPhysics { component(PlatformerPhysics.self)! }
    private var collision: CollisionBox { component(CollisionBox.self)! }
    private var transform: Transform { component(Transform.self)! }

    override func update(_ context: Node) {
        if Keyboard.isPressed(87) && collision.hasCollision() { physics.jump() }
        if Keyboard.isPressed(65) { transform.translate(x: -speed, y: 0) }
        if Keyboard.isPressed(83) {
            transform.translate(x: 0, y: 1.5 * physics.jumpSpeed)
            transform.delta = .zero
        }
        if Keyboard.isPressed(68) { transform.translate(x: speed, y: 0) }

        guard let game else { return }
        for collectible in game.descendants(Collectible.self) {
            guard let box = collectible.first(CollisionBox.self), collision.overlaps(box) else { continue }
            speed += 0.2
            game.removeChild(collectible)
        }
    }
}

private final class Collectible: Entity {
    init(_ parent: Scene, sheet: Spritesheet<SDLTexture>, position: Vec2<Float>) {
        super.init(parent)
        _ = sheet.load(self, position: (0, 1))
        _ = Transform(self, position: position)
        _ = CollisionBox(self, mode: .passThrough, size: Vec2(tileSize, tileSize))
    }
}

private final class Tile: Entity {
    init(_ parent: Scene, sheet: Spritesheet<SDLTexture>, position: Vec2<Float>) {
        super.init(parent)
        _ = sheet.load(self)
        _ = Transform(self, position: position)
        _ = CollisionBox(self, mode: .doNotAvoid, size: Vec2(tileSize, tileSize))
    }
}

private final class WorldGenerator: Component {
    private let sheet: Spritesheet<SDLTexture>
    private let scene: Scene
    private let camera: Camera
    private var generatedTo: Float = 0

    init(_ parent: Entity, scene: Scene, sheet: Spritesheet<SDLTexture>, camera: Camera) {
        self.scene = scene
        self.sheet = sheet
        self.camera = camera
        super.init(parent)
    }

    override func update(_ context: Node) {
        guard let cameraTransform = camera.first(Transform.self) else { return }
        let target = ceil(((cameraTransform.position.x + width) - (generatedTo + tileSize) + 64) / tileSize) * tileSize
        let distance = max(0, target - generatedTo)
        guard distance > 0 else { return }

        for x in stride(from: generatedTo, to: generatedTo + distance, by: tileSize) {
            for y in stride(from: Float(0), to: height, by: tileSize) {
                let value = Int.random(in: 0..<100)
                if value < 4 {
                    _ = Tile(scene, sheet: sheet, position: Vec2(x, y))
                } else if value < 6 {
                    _ = Collectible(scene, sheet: sheet, position: Vec2(x, y))
                }
            }
        }
        generatedTo += distance
    }
}

let game = Game()
let camera = Camera(game, screenSize: Vec2(800, 600))
let player = Entity(game, name: "player")
let transform = Transform(player)
let playerCollision = CollisionBox(player, mode: .doNotAvoid, size: Vec2(tileSize, tileSize))
camera.follow(transform, lerpFactor: 0.02)
let physics = PlatformerPhysics(player)
_ = PlayerController(player)
_ = TimerSystem(game)
let backend = SDLBackend()
let renderer = Renderer(game, title: "Don't Look Back", width: 800, height: 600, backend: backend)
let image = Bundle.module.path(forResource: "tilemap", ofType: "bmp")!
let assets = Entity(game, name: "assets")
let sheet = Spritesheet<SDLTexture>(assets, image: image, imageSize: Vec2(tileSize, tileSize)) {
    try backend.loadBMP($0)
}
_ = sheet.load(player, position: (0, 5))
_ = playerCollision
_ = physics
_ = Tile(game, sheet: sheet, position: Vec2(0, 50))
_ = Collectible(game, sheet: sheet, position: Vec2(-50, -50))
let generatorEntity = Entity(game, name: "generator")
_ = WorldGenerator(generatorEntity, scene: game, sheet: sheet, camera: camera)
_ = Painter<SDLTexture>(renderer) { texture, source, destination in
    backend.draw(texture, source: source, destination: destination)
}
do { try renderer.run() } catch { print("Don't Look Back example failed: \(error)") }
