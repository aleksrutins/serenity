#pragma once

#include <atomic>
#include <sup.hpp>
#include <SDL3/SDL.h>

namespace serenity {
namespace graphics {

class Renderer : public Sup {
    SDL_Window *win;
    SDL_Renderer *ren;

    const char *title;
    Uint64 width;
    Uint64 height;

    std::atomic_bool _quit = false;

public:
	SUP_ISA(Renderer)

    Renderer(Sup *parent, const char *title, Uint64 width = 640, Uint64 height = 480)
    	: Sup(parent)
    	, title(title)
    	, width(width)
    	, height(height) {}

    inline auto renderer() {return ren;}
    void run();
    void quit();
};

}
}
