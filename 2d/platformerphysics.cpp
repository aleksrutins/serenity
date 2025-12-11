#include <serenity/core.hpp>
#include <serenity/2d.hpp>

using namespace serenity;
using serenity::time::TimerSystem;
using namespace serenity::math;

namespace serenity {
namespace twod {

void PlatformerPhysics::update(Sup *s) {
    auto t = dynamic_cast<TimerSystem*>(s);
    if(!t) return;

    auto box = getComponent<CollisionBox>();
    auto tr = getComponent<Transform>();
    if(!(box && tr)) return;

    if(!box->hasCollision()) {
        tr->delta[y] += fallSpeed;
    } else {
        tr->delta[y] = 0;
    }
}

void PlatformerPhysics::jump() {
    auto tr = getComponent<Transform>();
    if(!tr) return;

    tr->delta[y] -= jumpSpeed;
}

}
}
