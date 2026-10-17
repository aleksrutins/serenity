#pragma once
#include <sup.hpp>

namespace serenity {

class Entity;

class Scene : public Sup
{
public:
	SUP_ISA(Scene)

    explicit Scene(Sup *parent) : Sup(parent) {}
    auto entities() -> std::vector<Entity*>;
};

}
