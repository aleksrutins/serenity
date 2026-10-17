#pragma once
#include <serenity/component.hpp>
#include <functional>

namespace serenity {

template <typename S>
class Passthrough : public serenity::Component
{
    std::function<void(S*)> _init;
    std::function<void(S*)> _update;
public:
	SUP_ISA(Passthrough<S>, Component)

    Passthrough(Entity *e, decltype(_init) init, decltype(_update) update)
    	: Component(e)
    	, _init(init)
    	, _update(update) {}

    void init(Sup *s) {
    	S *expected = dynamic_cast<S*>(s);
    	if(expected) _init(expected);
    };

    void update(Sup *s) {
    	S *expected = dynamic_cast<S*>(s);
    	if(expected) _update(expected);
    };

    static void none(S*){};
};

}
