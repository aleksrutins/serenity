#pragma once
#include <serenity/component.hpp>
#include <functional>

namespace serenity {
namespace time {
class TimerSystem;

class OnTick : public serenity::Component
{
    std::function<void(TimerSystem*)> _onTick;
public:
    OnTick(Entity *e, decltype(_onTick) onTick) : Component(e), _onTick(onTick) {}

    void init(Sup *);
    void update(Sup *);
};

}
}