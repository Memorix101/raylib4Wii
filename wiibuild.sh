#!/bin/sh
set -e

# Check if $DEVKITPRO is set.
if test ! $DEVKITPRO; then { echo "ERROR: Set \$DEVKITPRO installing devkitpro before continuing."; exit 1; } fi

# Check if DEVKITPPC is set.
if test ! $DEVKITPPC; then { echo "ERROR: Set \$DEVKITPPC installing gamecube toolchain  before continuing."; exit 1; } fi

# Resolve the directory this script lives in (the raylib4Wii repo root), so the
# patch and platform backend are found regardless of the current working directory.
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# Clone the upstream raylib4Consoles 6.0 branch, then add the Wii support on top.
# Skip the clone/patch step if ./raylib already exists (re-applying the patch on an
# already-patched tree would fail) - delete the folder for a clean setup.
if test ! -d raylib; then
    git clone https://github.com/raylib4Consoles/raylib -b raylib4Consoles_6.0

    git -C raylib apply "$SCRIPT_DIR/raylib_wii.patch"

    # Copy the Wii platform backend
    cp "$SCRIPT_DIR/src/platforms/rcore_wii.c" raylib/src/platforms/
else
    echo "NOTE: ./raylib already exists - skipping clone/patch/copy."
    echo "      Delete the folder if you want a clean, freshly patched checkout."
fi

cd raylib/src
make PLATFORM=PLATFORM_WII
make PLATFORM=PLATFORM_WII install
