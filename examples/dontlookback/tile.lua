return function(game, sprites, constants, x, y)
	local entity = Entity.new(game)
	sprites:load(entity, vec.vec2i(0, 0))
	Transform.new(entity, vec.vec2f(x, y))
	CollisionBox.new(entity, CollisionBox.DoNotAvoid, vec.vec2f(constants.TILE_SIZE, constants.TILE_SIZE))
	return entity
end
