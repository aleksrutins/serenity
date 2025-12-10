#pragma once

#include <SDL3/SDL.h>
#include <sup.hpp>
#include <serenity/graphics.hpp>

namespace serenity {
namespace twod {

class Painter : public Sup {
public:
	Painter(serenity::graphics::Renderer *);

	inline auto rectangle(float x, float y, float w, float h) -> SDL_FRect {
	    SDL_FRect rect;
	    rect.x = x;
	    rect.y = y;
	    rect.w = w;
	    rect.h = h;
	    return rect;
	}

	auto loadImage(const char *path) -> SDL_Texture*;

	void draw(SDL_Texture*, const SDL_FRect*);
};

}
}