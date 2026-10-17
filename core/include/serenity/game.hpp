#pragma once
#include <sup.hpp>
#include <vector>
#include "scene.hpp"

namespace serenity {

class System;

class Game : public Scene {
public:
	SUP_ISA(Game, Scene)

	explicit Game(Sup *parent = nullptr) : Scene(parent) {}
};

}
