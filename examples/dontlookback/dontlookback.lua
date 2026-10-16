#!/usr/bin/env serenity

local constants = require("examples.dontlookback.constants")
local make_tile = require("examples.dontlookback.tile")
local make_collectible = require("examples.dontlookback.collectible")
local make_player = require("examples.dontlookback.player")
local make_generator = require("examples.dontlookback.generator")

local game = Game.instance
local timer = TimerSystem.new(game)
local camera = Camera.new(game, vec.vec2f(constants.WIDTH, constants.HEIGHT))
local sprites = Spritesheet.for_scene(game, constants.TILEMAP, vec.vec2f(constants.TILE_SIZE, constants.TILE_SIZE))

make_tile(game, sprites, constants, 0, 50)

local collectibles = {}
make_collectible(game, sprites, constants, collectibles, -50, -50)

local player = make_player(game, sprites, camera, collectibles, constants)
camera:follow(player, 0.02)

make_generator(game, sprites, constants, collectibles, camera)

local renderer = Renderer.new(game, "Don't Look Back", constants.WIDTH, constants.HEIGHT)
Painter.new(renderer)

OnTick.new(player, OnTick.none, function()
	if key("q") then
		renderer:quit()
	end
end)

timer:start120()
renderer:run()
