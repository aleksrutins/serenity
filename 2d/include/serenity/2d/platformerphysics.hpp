#pragma once
#include <serenity/core.hpp>
#include <SDL3/SDL.h>
#include <optional>

namespace serenity {
namespace twod {

class PlatformerPhysics : public serenity::Component
{
public:
	float fallSpeed;
	float jumpSpeed;

	PlatformerPhysics(serenity::Entity *e, float fallSpeed = .05, float jumpSpeed = 3)
		: Component(e)
		, fallSpeed(fallSpeed)
		, jumpSpeed(jumpSpeed) {}

	void update(Sup *s);

	void jump();
};

}
}