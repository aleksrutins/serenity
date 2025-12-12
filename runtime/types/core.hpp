lua.new_usertype<Sup>("Sup");

lua.new_usertype<Game>("Game",
    sol::base_classes, sol::bases<Scene, Sup>()
);

lua["Game"]["instance"] = &app;

lua.new_usertype<Entity>("Entity",
    "new", overload(
    	[](Scene *s){return new Entity(s);},
    	[](Scene *s, string name){return new Entity(s, name);}
    ),
    sol::base_classes, sol::bases<Sup>()
);

lua.new_usertype<Component>("Component",
    "new", factories([](Sup *s){return new Component(s);}),
    sol::base_classes, sol::bases<Sup>()
);

lua.new_usertype<time::TimerSystem>("TimerSystem",
    "new", factories([](Sup *s){return new time::TimerSystem(s);}),
    "start60", &time::TimerSystem::start<60>,
    "start120", &time::TimerSystem::start<120>,
    "stop", &time::TimerSystem::stop,
    sol::base_classes, sol::bases<Sup>()
);

lua.new_usertype<time::OnTick>("OnTick",
    "new", factories([](Entity *e, std::function<void(time::TimerSystem*)> init, std::function<void(time::TimerSystem*)> update){return new time::OnTick(e, init, update);}),
    sol::base_classes, sol::bases<Sup>()
);

lua["OnTick"]["none"] = time::OnTick::none;