#!/usr/bin/env serenity

local WIDTH, HEIGHT = 800, 600
local TILE_SIZE = 32
local TILEMAP = "examples/dontlookback/tilemap.png"

local game = Game.instance
local timer = TimerSystem.new(game)
local camera = Camera.new(game, vec.vec2f(WIDTH, HEIGHT))
local sprites = Spritesheet.for_scene(game, TILEMAP, vec.vec2f(TILE_SIZE, TILE_SIZE))

local function make_tile(x, y)
	local entity = Entity.new(game)
	sprites:load(entity, vec.vec2i(0, 0))
	Transform.new(entity, vec.vec2f(x, y))
	CollisionBox.new(entity, CollisionBox.DoNotAvoid, vec.vec2f(TILE_SIZE, TILE_SIZE))
	return entity
end

local collectibles = {}
local function make_collectible(x, y)
	local entity = Entity.new(game)
	sprites:load(entity, vec.vec2i(0, 1))
	local box = CollisionBox.new(entity, CollisionBox.PassThrough, vec.vec2f(TILE_SIZE, TILE_SIZE))
	Transform.new(entity, vec.vec2f(x, y))
	collectibles[#collectibles + 1] = { entity = entity, box = box }
end

make_tile(0, 50)
make_collectible(-50, -50)

local player = Entity.new(game, "smiley")
sprites:load(player, vec.vec2i(0, 5))
local transform = Transform.new(player)
local player_box = CollisionBox.new(player, CollisionBox.DoNotAvoid)
local physics = PlatformerPhysics.new(player)
local speed = 4
local player_x = 0
local renderer

camera:follow(player, 0.02)

OnTick.new(player, OnTick.none, function()
	if key("w") and player_box:hasCollision() then
		physics:jump()
	end
	if key("a") then
		transform:translate(-speed, 0)
		player_x = player_x - speed
	end
	if key("s") then
		transform:translate(0, 1.5 * physics.jumpSpeed)
	end
	if key("d") then
		transform:translate(speed, 0)
		player_x = player_x + speed
	end
	if key("q") then
		renderer:quit()
	end

	for i = #collectibles, 1, -1 do
		local collectible = collectibles[i]
		if player_box:overlaps(collectible.box) then
			speed = speed + 0.2
			print("[collectible] " .. speed)
			camera:follow(player, 0.005 * speed)
			collectible.entity:remove()
			collectibles[i] = collectibles[#collectibles]
			collectibles[#collectibles] = nil
		end
	end
end)

local generator = Entity.new(game)
local generated_to = 0
local random_state = 1
OnTick.new(generator, OnTick.none, function()
	local required = ((player_x + WIDTH) - (generated_to + TILE_SIZE) + 64)
	local distance = ((required + TILE_SIZE - 1) // TILE_SIZE) * TILE_SIZE
	if distance > 0 then
		for x = 0, distance - TILE_SIZE, TILE_SIZE do
			for y = 0, HEIGHT - TILE_SIZE, TILE_SIZE do
				random_state = (random_state * 48271) % 2147483647
				local roll = random_state % 100
				if roll < 4 then
					make_tile(generated_to + x, y)
				elseif roll < 6 then
					make_collectible(generated_to + x, y)
				end
			end
		end
	end
	generated_to = generated_to + distance
end)

renderer = Renderer.new(game, "Don't Look Back", WIDTH, HEIGHT)
Painter.new(renderer)

timer:start120()
renderer:run()
