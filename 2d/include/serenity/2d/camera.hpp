#pragma once
#include <serenity/core.hpp>

namespace serenity {
namespace twod {

class Transform;

class Camera : public serenity::Entity
{
	double lerpFactor = 0;
	Transform *following = nullptr;
public:
	Camera(serenity::Scene *, std::optional<std::string> = std::nullopt);

	void follow(Transform *tr, double lerpFactor);
	void follow(serenity::Entity *ent, double lerpFactor);
};

}
}