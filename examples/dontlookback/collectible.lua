return function(game, sprites, constants, collectibles, x, y)
	local entity = Entity.new(game)
	sprites:load(entity, vec.vec2i(0, 1))
	local box = CollisionBox.new(entity, CollisionBox.PassThrough, vec.vec2f(constants.TILE_SIZE, constants.TILE_SIZE))
	Transform.new(entity, vec.vec2f(x, y))
	collectibles[#collectibles + 1] = { entity = entity, box = box }
	return entity
end
