#pragma once

#include <atomic>
#include <functional>
#include <utility>
#include <sup.hpp>
#include <SDL3/SDL.h>

namespace serenity {
namespace graphics {

class [[sup]] Renderer : public Sup {
    SDL_Window *win;
    SDL_Renderer *ren;

    const char *title;
    Uint64 width;
    Uint64 height;

    std::atomic_bool _quit = false;
    std::function<void(SDL_Event *)> eventHandler;

public:

    Renderer(Sup *parent, const char *title, Uint64 width = 640, Uint64 height = 480)
    	: Sup(parent)
    	, title(title)
    	, width(width)
    	, height(height) {}

	auto *window() {return win;}
    auto *renderer() {return ren;}
    void setEventHandler(std::function<void(SDL_Event *)> handler) {
        eventHandler = std::move(handler);
    }
    void run();
    void quit();
};

}
}
