#include <iostream>

#include <serenity/core.hpp>
#include <serenity/graphics.hpp>
#include <serenity/2d.hpp>

using namespace std;
using namespace std::chrono;
using namespace serenity;
using namespace serenity::time;
using namespace serenity::graphics;
using namespace serenity::twod;

int main() {
    Game g;

    new TimerSystem(&g);

    new OnTick(new Entity(&g, "ticker"), OnTick::none, [](TimerSystem *t) {
        cout << "Frame: "
             << t->delta().count()
             << "ms\n";
    });

    auto r = new Renderer(&g, "Serenity Example");
    auto p = new Painter(r);

    SDL_Texture *tex;

    new CustomRender(new Entity(&g, "lettuce"), [&](Renderer *) {
        tex = p->loadImage(LETTUCE_PATH);
    }, [&](Renderer *) {
        p->draw(tex, nullptr);
    });

    r->run();

    g.findChild<TimerSystem>()->start<1>();

    return 0;
}
