#!/bin/bash

set -e

echo "Setting up ANGLE for macOS..."

ANGLE_DIR="angle"

if [ -d "$ANGLE_DIR" ]; then
    echo "ANGLE directory already exists. Remove it to reinstall."
    exit 1
fi

echo "Creating ANGLE directory structure..."
mkdir -p "$ANGLE_DIR/include"
mkdir -p "$ANGLE_DIR/lib"

echo ""
echo "=========================================="
echo "ANGLE Setup for macOS"
echo "=========================================="
echo ""
echo "Attempting to download pre-built ANGLE libraries..."
echo ""

cd "$ANGLE_DIR"

echo "Cloning ANGLE repository (shallow, headers only)..."
if ! git clone --depth 1 --branch chromium/6478 https://chromium.googlesource.com/angle/angle angle_src 2>/dev/null; then
    echo "Failed to clone ANGLE. Trying alternative source..."
    git clone --depth 1 https://chromium.googlesource.com/angle/angle angle_src
fi

echo "Copying ANGLE headers..."
cp -r angle_src/include/* include/

echo "Cleaning up source..."
rm -rf angle_src

echo ""
echo "=========================================="
echo "Building ANGLE libraries..."
echo "=========================================="
echo ""
echo "ANGLE requires building from source on macOS."
echo ""
echo "Please run the following commands to build ANGLE:"
echo ""
echo "  cd $ANGLE_DIR"
echo "  git clone https://chromium.googlesource.com/angle/angle"
echo "  cd angle"
echo "  python scripts/bootstrap.py"
echo "  gclient sync"
echo "  gn gen out/Release --args='is_debug=false target_cpu=\"x64\"'"
echo "  autoninja -C out/Release"
echo "  cd .."
echo "  cp angle/out/Release/libEGL.dylib lib/"
echo "  cp angle/out/Release/libGLESv2.dylib lib/"
echo ""
echo "Alternatively, you can try using MoltenVK or download binaries from:"
echo "  https://github.com/google/angle"
echo ""
echo "=========================================="
