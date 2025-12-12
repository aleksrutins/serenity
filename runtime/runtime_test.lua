#!/usr/bin/env serenity

local g = Game.instance

local t = TimerSystem.new(g)

local sprites = Spritesheet.for_scene(g, "/app/share/serenity/examples/tilemap.png", vec.vec2f(32, 32))

local e2 = Entity.new(g, "follower")
local tr2 = Transform.new(e2, vec.vec2f(50, 50))
sprites:load(e2, vec.vec2i(0, 5))

local e = Entity.new(g, "player")
local tr = Transform.new(e, vec.vec2f(50, 50))
sprites:load(e, vec.vec2i(0, 1))

local r = Renderer.new(g, "Hello World", 800, 600)
Painter.new(r)

OnTick.new(e, OnTick.none, function(t)
	if key('w') then
		tr:translate(0, -5)
	end
	if key('s') then
		tr:translate(0, 5)
	end
	if key('a') then
		tr:translate(-5, 0)
	end
	if key('d') then
		tr:translate(5, 0)
	end

	if key('q') then
		r:quit()
	end

	tr2:position(math.lerp(tr2:position(), tr:position(), 0.01))
end)

print("hi")

t:start120()

r:run()