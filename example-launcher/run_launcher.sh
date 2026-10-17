#!/bin/sh
set -eu

workspace="${BUILD_WORKSPACE_DIRECTORY:-$(pwd)}"
cd "$workspace"
exec "$workspace/bazel-bin/runtime/serenity" "$workspace/example-launcher/launcher.lua"
