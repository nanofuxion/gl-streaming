# macOS Build Notes

## Summary
Successfully configured the GL server to build on macOS using ANGLE for OpenGL ES support.

## Changes Made

### 1. Makefile (`gl_server/Makefile`)
- Added automatic macOS detection using `uname -s`
- When building on macOS:
  - Sets `USE_ANGLE` preprocessor flag
  - Includes ANGLE headers from `../angle/include`
  - Links against ANGLE libraries (`libEGL.dylib`, `libGLESv2.dylib`)
  - Links required macOS frameworks (Metal, CoreFoundation, IOKit, CoreGraphics, Cocoa)
  - Suppresses int/pointer conversion warnings (`-Wno-int-conversion -Wno-incompatible-pointer-types`)
  - Provides helpful error message if ANGLE is not installed

### 2. Header Files
- **`gl_server/glcontrol.h`**: Added conditional includes for ANGLE headers
  - Uses system-style includes (`<GLES2/gl2.h>`) on macOS with `USE_ANGLE`
  - Uses local includes (`"GLES2/gl2.h"`) on other platforms

- **`gl_server/glserver.h`**: Added declaration for `glse_cmd_send_data` function

### 3. Source Files
- **`gl_server/glcontrol.c`**: Fixed error handling return types and format strings
- **`gl_server/serveregl.c`**: Added `#include <string.h>` for `strncpy`
- **`gl_server/servergles.c`**: 
  - Added `#include <string.h>` for string functions
  - Added stub implementations for `glMapBufferOES` and `glUnmapBufferOES` (not available in ANGLE)

### 4. ANGLE Setup
- Created `setup_angle_macos.sh` script to download ANGLE headers
- Copied ANGLE libraries from Visual Studio Code's embedded Chromium build
- Located at: `angle/lib/libEGL.dylib` and `angle/lib/libGLESv2.dylib`

## Build Instructions

```bash
cd gl_server
make
```

The Makefile automatically:
1. Detects macOS platform
2. Checks for ANGLE installation
3. Configures appropriate compiler flags
4. Links against ANGLE libraries

## Known Limitations

### Architecture Compatibility
The codebase was designed for 32-bit ARM systems where:
- Pointers fit in `uint32_t` (32-bit integers)
- EGL handles can be transmitted over the network protocol as 32-bit values

On 64-bit macOS:
- Pointers are 64-bit but stored/transmitted as 32-bit
- This causes pointer truncation at runtime
- **The server compiles but may not function correctly for client-server communication**

### OpenGL ES Extensions
Some OpenGL ES extensions are not available in ANGLE and have been stubbed:
- `glMapBufferOES` - returns NULL
- `glUnmapBufferOES` - returns GL_FALSE

These functions are part of the OES_mapbuffer extension and are not supported on Metal backend.

## Recommended Usage

- ✅ **GL Client on macOS**: Fully functional
- ⚠️ **GL Server on macOS**: Experimental - compiles but has runtime limitations
- ✅ **GL Server on Android/Raspberry Pi**: Recommended for production use

## Future Improvements

To fully support 64-bit architectures:
1. Update network protocol to handle 64-bit handles
2. Use `uintptr_t` instead of `uint32_t` for pointer storage
3. Implement proper serialization for platform-independent handle transfer
4. Consider using handle tables with 32-bit indices instead of raw pointers
