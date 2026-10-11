// swift-tools-version: 6.0
import PackageDescription

let package = Package(
    name: "Serenity",
    platforms: [
        .macOS(.v13),
        .iOS(.v16),
        .tvOS(.v16)
    ],
    products: [
        .library(name: "Serenity", targets: ["Serenity"]),
        .library(name: "Serenity2D", targets: ["Serenity2D"]),
        .library(name: "SerenitySDL", targets: ["SerenitySDL"]),
        .executable(name: "serenity-runtime", targets: ["SerenityRuntime"]),
        .executable(name: "serenity-launcher", targets: ["SerenityLauncher"]),
        .executable(name: "serenity-example-camera", targets: ["ExampleCamera"]),
        .executable(name: "serenity-example-input", targets: ["ExampleInput"]),
        .executable(name: "serenity-example-dontlookback", targets: ["ExampleDontLookBack"]),
        .executable(name: "serenity-example-lettuce", targets: ["ExampleLettuce"])
    ],
    targets: [
        .target(name: "Serenity"),
        .target(name: "Serenity2D", dependencies: ["Serenity"]),
        .systemLibrary(
            name: "CLua",
            path: "Sources/CLua",
            pkgConfig: "lua5.4",
            providers: [.apt(["liblua5.4-dev"])]
        ),
        .systemLibrary(
            name: "CSDL3",
            path: "Sources/CSDL3",
            pkgConfig: "sdl3",
            providers: [.apt(["libsdl3-dev"])]
        ),
        .target(name: "SerenitySDL", dependencies: ["Serenity", "CSDL3"]),
        .executableTarget(name: "SerenityRuntime", dependencies: ["Serenity", "Serenity2D", "CLua"]),
        .executableTarget(name: "SerenityLauncher", dependencies: ["Serenity"]),
        .executableTarget(name: "ExampleCamera", dependencies: ["Serenity", "Serenity2D", "SerenitySDL"], resources: [.process("Resources")]),
        .executableTarget(name: "ExampleInput", dependencies: ["Serenity", "Serenity2D", "SerenitySDL"], resources: [.process("Resources")]),
        .executableTarget(name: "ExampleDontLookBack", dependencies: ["Serenity", "Serenity2D", "SerenitySDL"], resources: [.process("Resources")]),
        .executableTarget(name: "ExampleLettuce", dependencies: ["Serenity", "Serenity2D", "SerenitySDL"], resources: [.process("Resources")]),
        .testTarget(name: "SerenityTests", dependencies: ["Serenity", "Serenity2D", "SerenitySDL"])
    ]
)
