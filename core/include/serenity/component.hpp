#pragma once
#include <sup.hpp>
#include <string>
#include <optional>

#include "entity.hpp"
#include "game.hpp"

namespace serenity {

class [[sup]] Component : public Sup {
protected:
	auto game() const -> Game* { return findParent<Game>(); }

	template <typename C>
	auto getComponent(std::optional<std::string> name = std::nullopt) const -> C* {
		auto e = findParent<Entity>();
		if(e) return e->findChild<C>(name);
		return nullptr;
	}

public:

	Component(Sup *parent) : Sup(parent) {}

	virtual void init(Sup *) {};
	virtual void update(Sup *) {};
};

}
