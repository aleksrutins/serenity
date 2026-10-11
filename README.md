# Serenity

Serenity is a cross-platform Swift Package. The `Serenity` library contains
the scene graph, math, rendering, timing, and input foundations. The separate
`Serenity2D` library contains transforms, cameras, collision, physics,
spritesheets, sprites, and painters, matching the original C++ target
boundary. The package
also provides `serenity-runtime`, `serenity-launcher`, and Swift versions of
the camera, input, don't-look-back, and lettuce examples.

Build and test with:

```sh
swift build
swift test
swift run serenity-runtime
```

Rendering and platform input are expressed through `RendererBackend`. The
`SerenitySDL` target uses a direct `CSDL3` system-library module map, following
SDL3's C API through Swift's C interop. It discovers SDL3 through `pkg-config`
(`libsdl3-dev` on Debian-based Linux systems). Lua scripting uses a direct
`CLua` system-library module map and Lua 5.4's C API, discovered through
`pkg-config` (`liblua5.4-dev` on Debian-based Linux systems). Scripts can read
the `entity_count` global.

The previous C++/Bazel implementation has been removed. Assets remain in the
example directories for backend implementations to consume.
