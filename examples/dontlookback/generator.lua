local math = require("math")

return function(game, sprites, constants, collectibles, camera)
	local generated_to = 0
	local random_state = 1
	local make_tile = require("examples.dontlookback.tile")
	local make_collectible = require("examples.dontlookback.collectible")
	local generator = Entity.new(game)

	OnTick.new(generator, OnTick.none, function()
		local required = (camera:x() + constants.WIDTH) - (generated_to + constants.TILE_SIZE) + 64
		local distance = math.floor(required / constants.TILE_SIZE + 0.5) * constants.TILE_SIZE
		if distance > 0 then
			for x = 0, distance - constants.TILE_SIZE, constants.TILE_SIZE do
				for y = 0, constants.HEIGHT - constants.TILE_SIZE, constants.TILE_SIZE do
					random_state = (random_state * 48271) % 2147483647
					local roll = random_state % 100
					if roll < 4 then
						make_tile(game, sprites, constants, generated_to + x, y)
					elseif roll < 6 then
						make_collectible(game, sprites, constants, collectibles, generated_to + x, y)
					end
				end
			end
			generated_to = generated_to + distance
		end
	end)

	return generator
end
