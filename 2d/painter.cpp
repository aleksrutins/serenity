#include <serenity/2d/painter.hpp>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

using namespace std;
using namespace serenity;
using namespace serenity::graphics;

namespace serenity::twod {

Painter::Painter(Renderer *r)
    : Sup(r)
{}

auto Painter::loadImage(const char *path) -> SDL_Texture* {
    auto tex = IMG_LoadTexture(dynamic_cast<Renderer*>(parent())->renderer(), path);
    if(!tex) return nullptr;
    manage([tex](){SDL_DestroyTexture(tex);});
    return tex;
}

void Painter::draw(SDL_Texture* tex, const SDL_FRect *dest) {
    draw(tex, nullptr, dest);
}

void Painter::draw(SDL_Texture* tex, const SDL_FRect *src, const SDL_FRect *dest) {
    auto r = dynamic_cast<Renderer*>(parent());
    if(!r) return;
    SDL_RenderTexture(r->renderer(), tex, src, dest);
}


}
