import Serenity
import Serenity2D
import SerenitySDL
import Foundation

let game = Game()
let player = Entity(game, name: "smiley")
let transform = Transform(player)
_ = TimerSystem(game)
let backend = SDLBackend()
let renderer = Renderer(game, title: "Serenity Input Example", backend: backend)
let image = Bundle.module.path(forResource: "smiley", ofType: "bmp")!
let assets = Entity(game, name: "assets")
let sheet = Spritesheet<SDLTexture>(assets, image: image, imageSize: Vec2(32, 32)) {
    try backend.loadBMP($0)
}
_ = sheet.load(player)
_ = Painter<SDLTexture>(renderer) { texture, source, destination in
    backend.draw(texture, source: source, destination: destination)
}
do { try renderer.run() } catch { print("Input example failed: \(error)") }
