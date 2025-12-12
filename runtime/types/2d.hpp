using namespace serenity::math;

lua["rectangle"] = overload(
	static_cast<SDL_FRect(*)(float, float, float, float)>(twod::rectangle),
	static_cast<SDL_FRect(*)(Vec2f, Vec2f)>(twod::rectangle)
);

lua.new_usertype<twod::Painter>("Painter",
    "new", factories([](graphics::Renderer *r){return new twod::Painter(r);}),
    "load_image", &twod::Painter::loadImage,
    "draw", overload(
    	static_cast<void(twod::Painter::*)(SDL_Texture*, const SDL_FRect*)>(&twod::Painter::draw),
    	static_cast<void(twod::Painter::*)(SDL_Texture*, const SDL_FRect*, const SDL_FRect*)>(&twod::Painter::draw)
    ),
    sol::base_classes, sol::bases<Sup>()
);

lua.new_usertype<twod::Spritesheet>("Spritesheet",
    "for_scene", factories(twod::Spritesheet::forScene),
    "load", &twod::Spritesheet::load,
    sol::base_classes, sol::bases<Component, Sup>()
);

lua.new_usertype<twod::Sprite>("Sprite",
    "load", factories(twod::Sprite::load),
    "size", &twod::Sprite::size,
    "box", &twod::Sprite::box,
    sol::base_classes, sol::bases<Component, Sup>()
);

lua.new_usertype<twod::Transform>("Transform",
    constructors<twod::Transform(Entity*, Vec2f)>(),
    base_classes, bases<Component, Sup>()
);