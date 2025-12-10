#include <serenity/core.hpp>
#include <serenity/graphics/renderer.hpp>

using namespace serenity;

namespace serenity::graphics {

void Renderer::run() {
    win = SDL_CreateWindow(title, 640, 480, 0);
    if(!win) throw 1;
    manage([this](){SDL_DestroyWindow(win);});

    ren = SDL_CreateRenderer(win, NULL);
    if(!ren) throw 1;
    manage([this](){SDL_DestroyRenderer(ren);});

    SDL_SetRenderVSync(ren, SDL_RENDERER_VSYNC_ADAPTIVE);

    auto g = findParent<Game>();

    for(auto e : g->children<Entity>()) e->init(this);

    SDL_Event e;
    bool quit = false;
    while(!quit) {
        while(SDL_PollEvent(&e)) if(e.type == SDL_EVENT_QUIT) quit = true;

        SDL_RenderClear(ren);
        for(auto e : g->children<Entity>()) e->update(this);
        SDL_RenderPresent(ren);
    }
}

void Renderer::quit() {
    SDL_Quit();
}

}
