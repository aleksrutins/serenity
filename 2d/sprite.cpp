#include <serenity/core.hpp>
#include <serenity/graphics.hpp>
#include <serenity/2d.hpp>
#include <SDL3/SDL.h>

using namespace serenity;
using namespace serenity::graphics;
using namespace serenity::math;

namespace serenity {
namespace twod {

Sprite::Sprite(Entity *e, const char *image, Vec2<float> size)
    : Component(e)
    , _image(image)
    , _size(size)
    {}

void Sprite::init(Sup *s) {
    auto r = dynamic_cast<Renderer*>(s);
    if(!r) return;
    auto p = r->findChild<Painter>();
	tex = p->loadImage(_image);
}

void Sprite::update(Sup *s) {
    auto r = dynamic_cast<Renderer*>(s);
    if(!r) return;
    auto p = r->findChild<Painter>();
    auto tr = getComponent<Transform>();

	auto target = rectangle(tr->screenPosition(), _size);
	if(tex) p->draw(tex, &target);
}

}
}
