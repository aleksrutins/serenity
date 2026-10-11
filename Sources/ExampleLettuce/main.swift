import Serenity
import Serenity2D
import SerenitySDL
import Foundation

let game = Game()
_ = TimerSystem(game)
let backend = SDLBackend()
let renderer = Renderer(game, title: "Serenity Example", backend: backend)
let image = Bundle.module.path(forResource: "lettuce", ofType: "bmp")!
let assetEntity = Entity(game, name: "lettuce")
let sheet = Spritesheet<SDLTexture>(assetEntity, image: image, imageSize: Vec2(492, 492)) {
    try backend.loadBMP($0)
}
let spriteEntity = Entity(game, name: "lettuce-sprite")
_ = Transform(spriteEntity)
_ = sheet.load(spriteEntity)
_ = Painter<SDLTexture>(renderer) { texture, source, destination in
    backend.draw(texture, source: source, destination: destination)
}
do { try renderer.run() } catch { print("Lettuce example failed: \(error)") }
