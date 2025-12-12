lua.new_usertype<graphics::Renderer>("Renderer",
    "new", factories([](Sup *s, const char *n, Uint64 w, Uint64 h){return new graphics::Renderer(s, n, w, h);}),
    "run", &graphics::Renderer::run,
    "quit", &graphics::Renderer::quit,
    sol::base_classes, sol::bases<Sup>()
);

lua.new_usertype<graphics::CustomRender>("CustomRender",
    "new", factories([](Entity *e, std::function<void(graphics::Renderer*)> init, std::function<void(graphics::Renderer*)> update){return new graphics::CustomRender(e, init, update);}),
    sol::base_classes, sol::bases<Component, Sup>()
);

lua["CustomRender"]["none"] = graphics::CustomRender::none;