import Foundation

let examples = [
    ("Lettuce", "serenity-example-lettuce"),
    ("Input", "serenity-example-input"),
    ("Camera", "serenity-example-camera"),
    ("Don't Look Back", "serenity-example-dontlookback"),
    ("Runtime", "serenity-runtime")
]

print("Serenity Examples")
for (index, example) in examples.enumerated() { print("\(index + 1). \(example.0) — \(example.1)") }
if let value = readLine(), let index = Int(value), examples.indices.contains(index - 1) {
    let process = Process()
    process.executableURL = URL(fileURLWithPath: "/usr/bin/env")
    process.arguments = [examples[index - 1].1]
    try process.run()
    process.waitUntilExit()
}
