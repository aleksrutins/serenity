"""Build-time C++ preprocessing helpers."""

def preprocess_cpp_files(name, files):
    """Return generated, mirrored copies of the given C++ sources and headers."""
    outputs = []
    for source in files:
        safe_name = source.replace("/", "_").replace(".", "_")
        output = "preprocessed/" + source
        native.genrule(
            name = name + "_" + safe_name,
            srcs = [source],
            tools = [
                "//codegen:sup_preprocess.lua",
                "//modules/lua5.4:lua-interpreter",
            ],
            outs = [output],
            cmd = "$(location //modules/lua5.4:lua-interpreter) " +
                  "$(location //codegen:sup_preprocess.lua) --input " +
                  "$(location " + source + ") -o $@",
        )
        outputs.append(output)
    return outputs
