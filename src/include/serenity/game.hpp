#pragma once
#include <sup.hpp>
#include <vector>
#include "scene.hpp"

namespace serenity {

class System;

class Game : public Scene {
public:
	explicit Game(Sup *parent = nullptr) : Scene(parent) {}
};

}