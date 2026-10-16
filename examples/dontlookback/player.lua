return function(game, sprites, camera, collectibles, constants)
	local player = Entity.new(game, "smiley")
	sprites:load(player, vec.vec2i(0, 5))
	local transform = Transform.new(player)
	local player_box = CollisionBox.new(player, CollisionBox.DoNotAvoid)
	local physics = PlatformerPhysics.new(player)
	local speed = 4

	OnTick.new(player, OnTick.none, function()
		if key("w") and player_box:hasCollision() then
			physics:jump()
		end
		if key("a") then
			transform:translate(-speed, 0)
		end
		if key("s") then
			transform:translate(0, 1.5 * physics.jumpSpeed)
		end
		if key("d") then
			transform:translate(speed, 0)
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

	return player
end
