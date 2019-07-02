# AngleViewer
OpenGL ES Viewer using ANGLE

# ToDo

* [ ] save window state when exit
* [ ] show background color

# Build
* Windows10(64bit)
* VisualStudio2017

## dependencies by vcpkg

* https://github.com/Microsoft/vcpkg

set vcpkg root path to environment variable `VCPKG_DIR`.

install packages.

`> vcpkg.exe install angle:x64-windows plog:x64-windows glm:x64-windows nlohmann-json:x64-windows`

## Create VisualStudio project by CMake

`CMAKE_INSTALL_PREFIX = %VCPKG_DIR%/installed/x64-windows`

# Execute

Add PATH to `%VCPKG_DIR%/installed/x64-windows/bin`
