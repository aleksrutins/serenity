#include <serenity/core.hpp>
#include <serenity/graphics/renderer.hpp>
#include <serenity/world.hpp>

using namespace serenity;
using namespace serenity::time;

namespace serenity::graphics {

void Renderer::run() {
    _quit.store(false);
    win = SDL_CreateWindow(title, width, height, 0);
    if(!win) throw 1;
    manage([this](){SDL_DestroyWindow(win);});

    ren = SDL_CreateRenderer(win, NULL);
    if(!ren) throw 1;
    manage([this](){SDL_DestroyRenderer(ren);});

    SDL_SetRenderVSync(ren, SDL_RENDERER_VSYNC_ADAPTIVE);

    auto g = findParent<Game>();

    {
        std::lock_guard<std::recursive_mutex> worldLock(serenity::worldMutex());
        for(auto e : g->children<Entity>()) e->init(this);
    }

    SDL_Event e;
    while(!_quit.load()) {
        while(SDL_PollEvent(&e)) {
            if(eventHandler) eventHandler(&e);
            if(e.type == SDL_EVENT_QUIT) _quit.store(true);
        }

        SDL_RenderClear(ren);
        {
            std::lock_guard<std::recursive_mutex> worldLock(serenity::worldMutex());
            for(auto e : g->children<Entity>()) e->update(this);
        }
        SDL_RenderPresent(ren);
    }

    for(auto timer : g->children<TimerSystem>()) timer->stop();
}

void Renderer::quit() {
    _quit.store(true);
    std::lock_guard<std::recursive_mutex> worldLock(serenity::worldMutex());
    for(auto timer : findParent<Game>()->children<TimerSystem>()) {
        timer->requestStop();
    }
}

}
