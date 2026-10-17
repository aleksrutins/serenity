#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>

#include <serenity/core.hpp>
#include <sol/sol.hpp>

namespace serenity::lua_runtime {

using type_caster = std::function<sol::object(sol::state_view, Sup*)>;

inline auto sup_type_casters() -> std::unordered_map<type_id, type_caster>& {
	static std::unordered_map<type_id, type_caster> casters;
	return casters;
}

template <typename T>
inline void register_sup_type(sol::state_view) {
	sup_type_casters()[type_id(typeid(T))] = [](sol::state_view lua, Sup* value) {
		return sol::make_object(lua, dynamic_cast<T*>(value));
	};
}

inline auto push_sup_as(sol::state_view lua, Sup* value, type_id type) -> sol::object {
	if (!value) return sol::make_object(lua, sol::lua_nil);
	auto caster = sup_type_casters().find(type);
	if (caster != sup_type_casters().end()) return caster->second(lua, value);
	return sol::make_object(lua, value);
}

inline auto push_sup(sol::state_view lua, Sup* value) -> sol::object {
	if (!value) return sol::make_object(lua, sol::lua_nil);
	return push_sup_as(lua, value, type_id(typeid(*value)));
}

inline auto children(Sup* self, sol::this_state state, type_id type) -> sol::table {
	sol::state_view lua(state);
	auto result = lua.create_table();
	int index = 1;
	for (auto child : self->children(type)) result[index++] = push_sup_as(lua, child, type);
	return result;
}

inline auto children_recursive(Sup* self, sol::this_state state, type_id type, bool recursive) -> sol::table {
	sol::state_view lua(state);
	auto result = lua.create_table();
	int index = 1;
	for (auto child : self->children(type, recursive)) result[index++] = push_sup_as(lua, child, type);
	return result;
}

inline auto find_child(Sup* self, sol::this_state state, type_id type) -> sol::object {
	return push_sup_as(sol::state_view(state), self->findChild(type), type);
}

inline auto find_child_named(Sup* self, sol::this_state state, type_id type, std::optional<std::string> name) -> sol::object {
	return push_sup_as(sol::state_view(state), self->findChild(type, std::move(name)), type);
}

inline auto find_child_recursive(Sup* self, sol::this_state state, type_id type, std::optional<std::string> name, bool recursive) -> sol::object {
	return push_sup_as(sol::state_view(state), self->findChild(type, std::move(name), recursive), type);
}

inline auto find_parent(Sup* self, sol::this_state state, type_id type) -> sol::object {
	return push_sup_as(sol::state_view(state), self->findParent(type), type);
}

inline auto find_parent_named(Sup* self, sol::this_state state, type_id type, std::optional<std::string> name) -> sol::object {
	return push_sup_as(sol::state_view(state), self->findParent(type, std::move(name)), type);
}

inline auto is_a(Sup* self, sol::this_state state, type_id type) -> sol::object {
	return sol::make_object(sol::state_view(state), self->isA(type));
}

} // namespace serenity::lua_runtime
