#pragma once
#include <sup.hpp>

namespace serenity {

class Entity;

class [[sup]] Scene : public Sup
{
public:

    explicit Scene(Sup *parent) : Sup(parent) {}
    auto entities() -> std::vector<Entity*>;
};

}
