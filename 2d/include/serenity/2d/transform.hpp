#pragma once
#include <serenity/core.hpp>
#include <SDL3/SDL.h>
#include <vector>

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

	serenity::math::Vec2f _position;

	std::vector<decltype(_position)> _history;
public:
	SUP_ISA(Transform, serenity::Component)

	Transform(serenity::Entity *e, decltype(_position) initialPos = serenity::math::vec2(0, 0)) : Component(e), _position(initialPos) {}

	auto screenPosition() {return screenPosition(cam());};
	auto screenPosition(Camera *) -> decltype(_position);

	auto position() -> serenity::math::Vec2f { return _position; }
	void position(decltype(_position) newPos);
	void translate(serenity::math::Vec2f vec) { position(_position + vec); }
	void translate(float x, float y) { translate(serenity::math::vec2(x, y)); }
	void rollback();

	serenity::math::Vec2f delta;
	void update(Sup *s);

	auto x() {return _position[serenity::math::x];}
	auto y() {return _position[serenity::math::y];}
};

}
}
