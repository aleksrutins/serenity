# Lua code generators

`bindgen.lua` reads a C++-oriented binding definition and writes a C++ header
containing sol2 registration code. Its implementation is split into
`lexer.lua`, `parser.lua`, and `emitter.lua`.

```sh
lua codegen/bindgen.lua runtime/types/math.idl -o /tmp/math.hpp
```

`vectorgen.lua` writes the named vector aliases, scalar aliases, and component
constructors. Bazel runs it to produce
`core/generated/serenity/math/vectypes.hpp`; the selected dimensions are set by
`VECTOR_DIMENSIONS` in `core/BUILD.bazel`:

```sh
lua codegen/vectorgen.lua --dimensions 2,3,4 -o /tmp/vectypes.hpp
```

`sup_preprocess.lua` walks a source directory recursively or processes a single
file. Bazel runs it on each project C++ source and header before compilation.
A class declaration marked `class [[sup]] Name : public Base` gets a public
`isA` override. The override checks `Name` and delegates to the first declared
base class.

```sh
lua codegen/sup_preprocess.lua --root . -o /tmp/serenity-preprocessed
```

Compile the mirrored source tree with its root (`/tmp/serenity-preprocessed`)
on the include path. The source files themselves are left untouched, so the
preprocessor can be rerun after edits. Both tools are exposed in
`//codegen:generators`.
