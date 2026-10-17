local examples = {
	{ name = "Lettuce", executable = "bazel-bin/examples/lettuce/serenity-example-lettuce" },
	{ name = "Input", executable = "bazel-bin/examples/input/serenity-example-input" },
	{ name = "Camera", executable = "bazel-bin/examples/camera/serenity-example-camera" },
	{
		name = "Don't Look Back (Lua)",
		executable = "bazel-bin/runtime/serenity",
		script = "examples/dontlookback/dontlookback.lua",
	},
	{
		name = "Runtime",
		executable = "bazel-bin/runtime/serenity",
		script = "runtime/runtime_test.lua",
	},
}

local function shell_quote(value)
	return "'" .. value:gsub("'", "'\\''") .. "'"
end

local function launch(example)
	local command = shell_quote(example.executable)
	if example.script then
		command = command .. " " .. shell_quote(example.script)
	end
	command = command .. " >/dev/null 2>&1 &"
	local ok = os.execute(command)
	return ok
end

local game = Game.instance
local renderer = Renderer.new(game, "Serenity Examples", 460, 560)
local initialized = false

local status = "Choose a scene to launch."

CustomRender.new(Entity.new(game), function(r)
        assert(r:init_imgui(), "Could not initialize Dear ImGui")
        imgui.style_colors_dark()
    end,
    function(r)
        sdl3.begin_imgui_frame()
        imgui.begin("Serenity Examples")
        imgui.text("SERENITY / PLAYGROUND")
        imgui.spacing()
        imgui.text("Examples")
        imgui.text(status)
        imgui.spacing()
        imgui.separator()
        imgui.spacing()

        for _, example in ipairs(examples) do
            if imgui.button(example.name) then
                if launch(example) then
                    status = "Launched " .. example.name .. "."
                else
                    status = "Could not launch " .. example.name .. "."
                end
            end
        end

        imgui.end_window()
        r:render_imgui_frame()
    end)

TimerSystem.new(game):start120()
renderer:run()