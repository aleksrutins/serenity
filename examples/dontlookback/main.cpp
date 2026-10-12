#include <iostream>

#include "constants.hpp"
#include "player.hpp"
#include "collectible.hpp"
#include "tile.hpp"
#include "generator.hpp"

#include <serenity/core.hpp>
#include <serenity/graphics.hpp>
#include <serenity/2d.hpp>
#include <serenity/input.hpp>

using namespace std;
using namespace std::chrono;
using namespace serenity;
using namespace serenity::time;
using namespace serenity::graphics;
using namespace serenity::twod;
using namespace serenity::input;

int main() {
    Game g;

    new TimerSystem(&g);

    auto cam = new Camera(&g, vec2(WIDTH, HEIGHT));

    auto sprites = Spritesheet::forScene(&g, TILEMAP_PATH, vec2(32, 32));

    new Tile(&g, sprites, vec2(0, 50));

    auto p = new Player(&g, sprites);
    cam->follow(p, 0.02);

    new Collectible(&g, sprites, vec2(-50, -50));

    new Generator(&g, sprites, cam);

    auto r = new Renderer(&g, "Don't Look Back", WIDTH, HEIGHT);
    new Painter(r);

    g.findChild<TimerSystem>()->start<120>();

    r->run();

    SDL_Quit();

    return 0;
}
