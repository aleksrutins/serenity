open class Node: AnyObject {
    public weak var parent: Node?
    private(set) public var children: [Node] = []
    private var cleanup: [() -> Void] = []
    public init(_ parent: Node? = nil) { self.parent = parent; parent?.addChild(self) }
    deinit { cleanup.forEach { $0() } }
    public func addChild(_ child: Node) { child.parent = self; children.append(child) }
    public func removeChild(_ child: Node) { children.removeAll { $0 === child }; child.parent = nil }
    public func descendants<T: Node>(_ type: T.Type = T.self) -> [T] {
        children.flatMap { child in
            [child as? T].compactMap { $0 } + child.descendants(type)
        }
    }
    public func first<T: Node>(_ type: T.Type = T.self, named name: String? = nil) -> T? {
        descendants(type).first { name == nil || ($0 as? NamedNode)?.name == name }
    }
    public func onCleanup(_ action: @escaping () -> Void) { cleanup.append(action) }
}

public protocol NamedNode { var name: String? { get } }

open class Component: Node {
    public var game: Game? { ancestor(Game.self) }
    public var entity: Entity? { ancestor(Entity.self) }
    open func initialize(_ context: Node) {}
    open func update(_ context: Node) {}
    public func component<T: Component>(_ type: T.Type = T.self) -> T? { entity?.first(type) }
    private func ancestor<T: Node>(_ type: T.Type) -> T? {
        var current = parent
        while let node = current {
            if let value = node as? T { return value }
            current = node.parent
        }
        return nil
    }
}

open class Scene: Node { public var entities: [Entity] { descendants(Entity.self) } }
public final class Game: Scene {}

open class Entity: Node, NamedNode {
    public let name: String?
    public init(_ parent: Scene, name: String? = nil) { self.name = name; super.init(parent) }
    open func initializeComponents(_ context: Node) { descendants(Component.self).forEach { $0.initialize(context) } }
    open func updateComponents(_ context: Node) { descendants(Component.self).forEach { $0.update(context) } }
}
