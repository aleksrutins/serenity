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
	Transform(serenity::Entity *e) : Component(e) {}

	serenity::math::Vec2<float> position;

	auto screenPosition() {return screenPosition(cam());};
	auto screenPosition(Camera *) -> decltype(position);
};

}
}