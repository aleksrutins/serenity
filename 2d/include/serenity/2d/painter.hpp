#pragma once

#include <SDL3/SDL.h>
#include <sup.hpp>
#include <serenity/core.hpp>
#include <serenity/graphics.hpp>

namespace serenity {
namespace twod {

inline auto rectangle(float x, float y, float w, float h) -> SDL_FRect {
    SDL_FRect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}

inline auto rectangle(serenity::math::Vec2f pos, serenity::math::Vec2f size) -> SDL_FRect {
    using serenity::math::x;
    using serenity::math::y;
    return rectangle(pos[x], pos[y], size[x], size[y]);
}

class Painter : public Sup {
public:
	Painter(serenity::graphics::Renderer *);

	auto loadImage(const char *path) -> SDL_Texture*;

	void draw(SDL_Texture*, const SDL_FRect*);
};

}
}