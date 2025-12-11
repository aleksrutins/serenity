#pragma once
#include <sup.hpp>

namespace serenity {

class Scene;

class Entity : public Sup {
	std::optional<std::string> _name;
public:
	Entity(Scene *parent, std::optional<std::string> name = std::nullopt);

	void init(Sup *);
	void update(Sup *);

	auto name() -> std::optional<std::string> {return _name;}
};

}