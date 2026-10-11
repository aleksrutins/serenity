import XCTest
@testable import Serenity
@testable import Serenity2D
@testable import SerenitySDL
#if os(Linux)
import Glibc
#else
import Darwin
#endif

final class SerenityTests: XCTestCase {
    func testCollisionAndRollback() {
        let game = Game()
        let scene = Scene(game)
        let first = Entity(scene)
        let second = Entity(scene)
        let firstTransform = Transform(first)
        _ = Transform(second, position: Vec2(4, 0))
        _ = CollisionBox(first, mode: .movable, size: Vec2(10, 10))
        _ = CollisionBox(second, mode: .solid, size: Vec2(10, 10))
        firstTransform.translate(x: 5, y: 0)
        XCTAssertTrue(first.first(CollisionBox.self)!.hasCollision())
        firstTransform.rollback()
        XCTAssertEqual(firstTransform.position, .zero)
    }

    func testSDLBackendCreatesWindow() throws {
        setenv("SDL_VIDEODRIVER", "dummy", 1)
        let backend = SDLBackend()
        try backend.createWindow(title: "Serenity Test", width: 32, height: 32)
        backend.shutdown()
    }
}
