def lua_binding(name):
    native.genrule(
        name = name + "_binding",
        srcs = ["types/" + name + ".idl"],
        outs = ["types/" + name + ".hpp"],
        tools = ["//bindgen:bindgen.py"],
        cmd = "python3 $(location //bindgen:bindgen.py) $(location types/" + name + ".idl) -o $@",
    )
