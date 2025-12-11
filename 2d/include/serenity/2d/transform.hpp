#pragma once
#include <serenity/core.hpp>
#include <SDL3/SDL.h>

namespace serenity {
namespace twod {

class Camera;

class Transform : public serenity::Component
{
	Camera *cam() {
		auto g = findParent<serenity::Game>();
		if(g) return g->findChild<Camera>();
		return nullptr;
	};
public:
	serenity::math::Vec2f position;

	Transform(serenity::Entity *e, decltype(position) initialPos = serenity::math::vec2(0, 0)) : Component(e), position(initialPos) {}

	auto screenPosition() {return screenPosition(cam());};
	auto screenPosition(Camera *) -> decltype(position);
};

}
}