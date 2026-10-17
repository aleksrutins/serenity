#include <sup.hpp>
#include <set>
#include <memory>
using namespace std;

static set<Sup *> __safe_ptrs;
class Yggdrasil : public Sup {
public:
    SUP_ISA(Yggdrasil)

    virtual optional<string> name() const {return "yggdrasil";}
    static Yggdrasil __;
};
Yggdrasil Yggdrasil::__;

Sup::Sup(Sup *parent)
    : _parent(parent)
{
    if(!_parent) _parent = &Yggdrasil::__;
    __safe_ptrs.insert(this);
    if(parent) {
        parent->addChild(this);
    }
}

Sup::~Sup() {
    __safe_ptrs.extract(this);
    if(_parent) _parent->remove(this);
    for (auto c : _children) {
        if(c && c != this && __safe_ptrs.contains(c)) {
            __safe_ptrs.extract(c);
            delete c;
        }
    }

    for (auto cb : _managed) {
        cb();
    }
}

bool Sup::isA(type_id const &what) const
{
	return what == type_id(typeid(Sup));
}

void Sup::manage(std::function<void()> freeCb) {
    _managed.push_back(freeCb);
}

void Sup::remove(Sup *c) {
    erase(_children, c);
}

auto Sup::children(type_id const &t, bool recursive) const -> std::vector<Sup*>
{
    std::vector<Sup*> result;
    for(auto c : _children) {
        if(!c) continue;
        if(c->isA(t)) result.push_back(c);
        if(recursive) {
            auto ch = c->children(t, recursive);
            result.insert(result.end(), ch.begin(), ch.end());
        }
    }
    return result;
}

auto Sup::findChild(type_id const& t, optional<string> name, bool recursive) const -> Sup*
{
    auto ch = children(t, recursive);
    auto el = find_if(ch.begin(), ch.end(), [name](auto c) {return !name || name == c->name();});
    if(el != ch.end()) return *el;
    return nullptr;
}

auto Sup::findParent(type_id const& t, std::optional<std::string> name) const -> Sup*
{
    auto p = _parent;
    while(p) {
        if(p->isA(t) && (!name || name == p->name())) return p;
        p = p->_parent;
    }
    return nullptr;
}

void Sup::addChild(Sup *c) {
    c->_parent = this;
    _children.push_back(c);
}
