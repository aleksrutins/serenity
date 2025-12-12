#!/usr/bin/env serenity

local g = Game.instance

local sprites = Spritesheet.for_scene(g, "/app/share/serenity/examples/smiley.bmp", vec.vec2f(32, 32))

local e = Entity.new(g, "smiley")
Transform.new(e, vec.vec2f(50, 50))
sprites:load(e, vec.vec2i(0, 0))

local r = Renderer.new(g, "Hello World", 800, 600)
Painter.new(r)

print("hi")
r:run()