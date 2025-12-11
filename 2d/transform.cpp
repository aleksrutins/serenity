#include <serenity/core.hpp>
#include <serenity/2d.hpp>

using namespace serenity::math;

namespace serenity {
namespace twod {

auto Transform::screenPosition(Camera *cam) -> Vec2<float> {
    if(!cam) return position;
    return position - cam->findChild<Transform>()->position;
}

}
}
