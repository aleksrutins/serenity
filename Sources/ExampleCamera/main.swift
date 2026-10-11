import Serenity
import Serenity2D
import SerenitySDL
import Foundation

let game = Game()
let camera = Camera(game, screenSize: Vec2(800, 600))
let player = Entity(game, name: "smiley")
let transform = Transform(player)
camera.follow(transform, lerpFactor: 0.01)
_ = TimerSystem(game)
let backend = SDLBackend()
let renderer = Renderer(game, title: "Serenity Camera Example", width: 800, height: 600, backend: backend)
let image = Bundle.module.path(forResource: "smiley", ofType: "bmp")!
let assets = Entity(game, name: "assets")
let sheet = Spritesheet<SDLTexture>(assets, image: image, imageSize: Vec2(32, 32)) {
    try backend.loadBMP($0)
}
_ = sheet.load(player)
let painter = Painter<SDLTexture>(renderer) { texture, source, destination in
    backend.draw(texture, source: source, destination: destination)
}
do { try renderer.run() } catch { print("Camera example failed: \(error)") }
