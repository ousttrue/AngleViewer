# AngleViewer
OpenGL ES Viewer using ANGLE

# Build
* Windows10(64bit)
* VisualStudio2017

## Build ANGLE by vcpkg

* https://github.com/Microsoft/vcpkg

`> vcpkg.exe install angle:x64-windows`

## Create VisualStudio project by CMake

`CMAKE_INSTALL_PREFIX = %VCPKG_DIR%/installed/x64-windows`

# Execute

Add PATH to `%VCPKG_DIR%/installed/x64-windows/bin` or `%VCPKG_DIR%/installed/x64-windows/debug/bin`

