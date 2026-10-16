def lua_binding(name):
    native.genrule(
        name = name + "_binding",
        srcs = ["types/" + name + ".idl", "//bindgen:bindgen"],
        outs = ["types/" + name + ".hpp"],
        tools = ["//bindgen:bindgen.lua", "//modules/lua5.4:lua-interpreter"],
        cmd = "$(location //modules/lua5.4:lua-interpreter) $(location //bindgen:bindgen.lua) $(location types/" + name + ".idl) -o $@",
    )
