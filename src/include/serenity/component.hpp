#pragma once
#include <sup.hpp>

#include "entity.hpp"
#include "game.hpp"

namespace serenity {

class Component : public Sup {
protected:
	auto game() const -> Game* { return findParent<Game>(); }

	template <typename C>
	auto getComponent() const -> C* {
		auto e = findParent<Entity>();
		if(e) return e->findChild<C>();
		return nullptr;
	}

public:
	Component(Entity *parent) : Sup(parent) {}

	virtual void init(Sup *) = 0;
	virtual void update(Sup *) = 0;
};

}