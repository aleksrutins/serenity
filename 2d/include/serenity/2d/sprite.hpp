#pragma once
#include <serenity/core.hpp>

namespace serenity {
namespace twod {

class Transform;

class Sprite : public serenity::Component
{
	const char *_image;
	SDL_Texture *tex;

	serenity::math::Vec2f _size;

public:
	Sprite(serenity::Entity *e, const char *imageName, serenity::math::Vec2f);

	void init(Sup *);
	void update(Sup *);

	inline auto size() -> decltype(_size) {return _size;}
};

}
}