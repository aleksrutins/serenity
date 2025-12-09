#include <iostream>

#include <serenity/core.hpp>
using namespace std;
using namespace std::chrono;
using namespace serenity;
using namespace serenity::time;

int main() {
    Game g;

    new OnTick(new Entity(&g, "ticker"), [](TimerSystem *t) {
        cout << "Frame: "
             << t->delta().count()
             << "ms\n";
    });
    (new TimerSystem(&g))->start<1>();

    while(true);

    return 0;
}
