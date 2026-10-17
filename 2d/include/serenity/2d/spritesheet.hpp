#pragma once
#include <serenity/core.hpp>
#include <string>

namespace serenity {
namespace twod {

class Sprite;

class [[sup]] Spritesheet : public serenity::Component
{

	const char *_image;
	serenity::math::Vec2i _imageSize;
	SDL_Texture *tex;

	serenity::math::Vec2f _size;

	friend class Sprite;

public:

	Spritesheet(serenity::Entity*, const char *imageName, serenity::math::Vec2f imageSize);

	void init(Sup *s);

	auto name() -> std::optional<std::string>;
	auto load(serenity::Entity*, serenity::math::Vec2i position) -> Sprite*;

	static auto forScene(Scene *s, const char *imageName, serenity::math::Vec2f imageSize) -> Spritesheet*;
};

}
}
