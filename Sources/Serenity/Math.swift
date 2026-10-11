public struct Vec2<T: BinaryFloatingPoint & Sendable>: Equatable, Sendable {
    public var x: T
    public var y: T
    public init(_ x: T = 0, _ y: T = 0) { self.x = x; self.y = y }
    public static var zero: Self { Self() }
    public var isZero: Bool { x == 0 && y == 0 }
}

public struct Vec4<T: BinaryFloatingPoint & Sendable>: Equatable, Sendable {
    public var x: T; public var y: T; public var width: T; public var height: T
    public init(_ x: T = 0, _ y: T = 0, _ width: T = 0, _ height: T = 0) {
        self.x = x; self.y = y; self.width = width; self.height = height
    }
    public init(position: Vec2<T>, size: Vec2<T>) { self.init(position.x, position.y, size.x, size.y) }
}

public extension Vec2 {
    static func + (a: Self, b: Self) -> Self { Self(a.x + b.x, a.y + b.y) }
    static func - (a: Self, b: Self) -> Self { Self(a.x - b.x, a.y - b.y) }
    static prefix func - (v: Self) -> Self { Self(-v.x, -v.y) }
    static func * (a: Self, b: T) -> Self { Self(a.x * b, a.y * b) }
    static func / (a: Self, b: T) -> Self { Self(a.x / b, a.y / b) }
}

public func lerp<T: BinaryFloatingPoint & Sendable>(_ a: Vec2<T>, _ b: Vec2<T>, _ amount: T) -> Vec2<T> {
    a + (b - a) * amount
}

public enum Collision {
    public static func overlaps<T: BinaryFloatingPoint>(_ a: Vec4<T>, _ b: Vec4<T>) -> Bool {
        !(a.y > b.y + b.height || b.y > a.y + a.height ||
          a.x > b.x + b.width || b.x > a.x + a.width)
    }
}
