import Foundation
import Serenity
import Serenity2D

let game = Game()
let scene = Scene(game)
let player = Entity(scene, name: "player")
_ = Transform(player)
_ = CollisionBox(player, mode: .movable, size: Vec2(32, 32))

let script = CommandLine.arguments.dropFirst().first
    .flatMap { try? String(contentsOfFile: $0, encoding: .utf8) }
    ?? String(data: FileHandle.standardInput.readDataToEndOfFile(), encoding: .utf8)
    ?? "print('Serenity Swift runtime initialized')"

do {
    try LuaBindings.run(script: script, game: game)
} catch {
    print("Lua runtime error: \(error)")
    exit(EXIT_FAILURE)
}
