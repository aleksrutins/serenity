#include <serenity/core.hpp>
#include <serenity/2d.hpp>

using namespace std;
using namespace serenity;
using namespace serenity::time;
using namespace serenity::math;
using namespace serenity::twod;

namespace serenity {
namespace twod {

Camera::Camera(Scene *parent, Vec2f screenSize, optional<string> name)
    : Entity(parent, name)
{
    auto tr = new Transform(this, -(screenSize/2));

    new OnTick(this, OnTick::none, [tr, screenSize, this](TimerSystem*) {
        if(following) {
            tr->position(lerp(tr->position(), following->position() - (screenSize/2), lerpFactor));
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
