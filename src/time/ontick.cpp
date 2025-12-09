#include <serenity/time/ontick.hpp>

#include <serenity/time/timersystem.hpp>

using namespace std;
using namespace serenity;

namespace serenity::time {

void OnTick::init(Sup *) {}
void OnTick::update(Sup *s) {
    auto t = dynamic_cast<TimerSystem*>(s);
    if(!t) return;

    _onTick(t);
}

}
