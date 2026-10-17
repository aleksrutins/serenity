---
--- Created by aleks
--- DateTime: 10/16/26 11:31 PM
---

return function(game)
    local line = Entity.new(game)
    local tr = Transform.new(line, vec.vec2f(-100, 0))

    local speed = 1

    OnTick.new(line, OnTick.none, function(t)
        tr:translate(speed, 0)

        for _, e in ipairs(game:children(Entity.class)) do
            if not e or e:is_a(Camera.class) then goto continue end
            local etr = e:find_child(Transform.class)
            if not etr then goto continue end

            if etr:x() < tr:x() then
                e:remove()
            end
            ::continue::
        end
    end)
end
