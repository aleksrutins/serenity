#pragma once
#include <sup.hpp>

namespace serenity {

class Scene;

class Entity : public Sup {
	std::string _name;
public:
	Entity(Scene *parent, std::string name);

	void init(Sup *);
	void update(Sup *);

	auto name() -> std::optional<std::string> {return _name;}
};

}