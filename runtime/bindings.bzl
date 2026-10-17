def lua_binding(name):
    native.genrule(
        name = name + "_binding",
        srcs = ["types/" + name + ".idl", "//codegen:bindgen"],
        outs = ["types/" + name + ".hpp"],
        tools = ["//codegen:bindgen.lua", "//modules/lua5.4:lua-interpreter"],
        cmd = "$(location //modules/lua5.4:lua-interpreter) $(location //codegen:bindgen.lua) $(location types/" + name + ".idl) -o $@",
    )
    native.genrule(
        name = name + "_binding_preprocessed",
        srcs = [":" + name + "_binding"],
        outs = ["preprocessed/types/" + name + ".hpp"],
        tools = ["//codegen:sup_preprocess.lua", "//modules/lua5.4:lua-interpreter"],
        cmd = "$(location //modules/lua5.4:lua-interpreter) " +
              "$(location //codegen:sup_preprocess.lua) --input $(location :" +
              name + "_binding) -o $@",
    )
