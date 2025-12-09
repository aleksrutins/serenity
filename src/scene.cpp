#include <serenity/scene.hpp>
#include <serenity/entity.hpp>

using namespace std;

namespace serenity {

auto Scene::entities() -> vector<Entity*>
{
    vector<Entity*> res;
    for (auto c : children()) {
        auto e = dynamic_cast<Entity*>(c);
        if(e) res.push_back(e);
        else {
            auto s = dynamic_cast<Scene*>(c);
            auto se = s->entities();
            if(s) res.insert(res.end(), se.begin(), se.end());
        }
    }
    return res;
}

}
