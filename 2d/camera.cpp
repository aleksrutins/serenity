#include <serenity/core.hpp>
#include <serenity/2d.hpp>

using namespace std;
using namespace serenity;
using namespace serenity::time;
using namespace serenity::math;
using namespace serenity::twod;

namespace serenity {
namespace twod {

Camera::Camera(Scene *parent, optional<string> name)
    : Entity(parent, name)
{
    auto tr = new Transform(this);

    new OnTick(this, OnTick::none, [tr, this](TimerSystem*) {
        if(following) {
            tr->position[x] = lerp(tr->position[x], following->position[x], lerpFactor);
            tr->position[y] = lerp(tr->position[y], following->position[y], lerpFactor);
        }
    });
}

void Camera::follow(Transform *tr, double lf) {
    following = tr;
    lerpFactor = lf;
}

void Camera::follow(Entity *ent, double lf) {
    auto tr = ent->findChild<Transform>();
    if(tr) follow(tr, lf);
}

}
}
