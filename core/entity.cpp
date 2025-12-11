#include <serenity/entity.hpp>
#include <serenity/scene.hpp>
#include <serenity/component.hpp>
using namespace std;

namespace serenity {

Entity::Entity(Scene *parent, optional<string> name)
    : Sup(parent)
    , _name(name) {}

void Entity::init(Sup *s) {
    for(auto c : children<Component>(true)) {
        c->init(s);
    }
}

void Entity::update(Sup *s) {
    for(auto c : children<Component>(true)) {
        c->update(s);
    }
}


}
