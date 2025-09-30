# macOS Setup Guide for GL Streaming Server

## Overview
The GL streaming server requires OpenGL ES 2.0/3.0 support, which is not natively available on macOS. We use ANGLE (Almost Native Graphics Layer Engine) to provide this compatibility by translating OpenGL ES calls to Metal.

## Current Status
✅ **Build Successful!** The GL server now compiles on macOS with ANGLE support.

⚠️ **Important Runtime Limitations**: The GL server code was originally written for 32-bit ARM systems (Raspberry Pi, Android). While it now compiles on 64-bit macOS, there are architectural compatibility issues:

- EGL handles (EGLDisplay, EGLContext, etc.) are 64-bit pointers on macOS but the network protocol uses `uint32_t` (32-bit) to transfer these handles
- **The server will compile but may not work correctly at runtime** due to pointer truncation
- Some OpenGL ES extensions (like `glMapBufferOES`) are stubbed out and return failure

**Recommendation**: 
- ✅ Use the GL **client** on macOS (works great!)
- ⚠️ Use the GL **server** on Android or Raspberry Pi for full functionality
- 🧪 The macOS server build is experimental and suitable for local testing only

## Quick Setup (Headers Already Installed)

The setup script has already installed the ANGLE headers. You now need to get the ANGLE libraries.

### Option 1: Download Pre-built ANGLE Binaries (Easiest)

Visit one of these sources for pre-built ANGLE binaries:
1. **Chromium Build Artifacts**: Download from https://commondatastorage.googleapis.com/chromium-browser-snapshots/index.html
   - Select Mac platform
   - Download a recent build
   - Extract `libEGL.dylib` and `libGLESv2.dylib` from the Chromium.app bundle
   - Copy to `angle/lib/`

2. **ANGLE Releases**: Check https://github.com/google/angle for any available releases

### Option 2: Build from Source (Advanced)

Building ANGLE requires depot_tools and can take 30+ minutes:

```bash
# Install depot_tools
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
export PATH="$PATH:$(pwd)/depot_tools"

# Build ANGLE
cd angle
git clone https://chromium.googlesource.com/angle/angle
cd angle
python scripts/bootstrap.py
gclient sync
gn gen out/Release --args='is_debug=false target_cpu="x64"'
autoninja -C out/Release

# Copy libraries
cd ../..
cp angle/angle/out/Release/libEGL.dylib angle/lib/
cp angle/angle/out/Release/libGLESv2.dylib angle/lib/
```

### Option 3: Use Alternative Compatibility Layers

Consider using:
- **MoltenVK**: Vulkan implementation on Metal (if your code can be adapted)
- **SwiftShader**: CPU-based renderer (slower but works without GPU)

## Building the Server

Once you have the libraries in `angle/lib/`, simply run:

```bash
cd gl_server
make
```

The Makefile will automatically detect macOS and use ANGLE.

## Troubleshooting

### Library Loading Issues
If you get library loading errors at runtime, you may need to adjust the library paths:

```bash
install_name_tool -id @rpath/libEGL.dylib angle/lib/libEGL.dylib
install_name_tool -id @rpath/libGLESv2.dylib angle/lib/libGLESv2.dylib
```

### Metal Backend
Ensure your Mac supports Metal (macOS 10.13+, most Macs from 2012 onwards).

## Alternative: X11 Mode

If you have X11 installed via XQuartz, you can try building with X11 support:

```bash
cd gl_server
make CFLAGS="$CFLAGS -DUSE_X11" LIBS="$LIBS -lX11"
```

This may work with the system's OpenGL implementation, though it's deprecated on macOS.
