import CLua
import Serenity

public enum LuaError: Error, CustomStringConvertible {
    case stateCreationFailed
    case executionFailed(String)

    public var description: String {
        switch self {
        case .stateCreationFailed:
            "Lua state creation failed"
        case let .executionFailed(message):
            "Lua execution failed: \(message)"
        }
    }
}

public enum LuaBindings {
    public static func run(script: String, game: Game) throws {
        guard let state = luaL_newstate() else {
            throw LuaError.stateCreationFailed
        }
        defer { lua_close(state) }

        luaL_openlibs(state)
        lua_pushinteger(state, lua_Integer(game.entities.count))
        "entity_count".withCString { lua_setglobal(state, $0) }

        let loadStatus = script.withCString { luaL_loadstring(state, $0) }
        guard loadStatus == LUA_OK else {
            throw LuaError.executionFailed(stackMessage(state))
        }

        let callStatus = lua_pcallk(state, 0, 0, 0, 0, nil)
        guard callStatus == LUA_OK else {
            throw LuaError.executionFailed(stackMessage(state))
        }
    }

    private static func stackMessage(_ state: OpaquePointer) -> String {
        guard let message = lua_tolstring(state, -1, nil) else {
            return "unknown Lua error"
        }
        return String(cString: message)
    }
}
