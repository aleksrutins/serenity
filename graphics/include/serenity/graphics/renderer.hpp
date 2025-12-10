#pragma once

#include <sup.hpp>
#include <SDL3/SDL.h>

namespace serenity {
namespace graphics {

class Renderer : public Sup {
    SDL_Window *win;
    SDL_Renderer *ren;

    const char *title;

public:
    Renderer(Sup *parent, const char *title) : Sup(parent), title(title) {}

    inline auto renderer() {return ren;}
    void run();
    void quit();
};

}
}