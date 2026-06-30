# Large Thin Wrapper
A thin OpenGL core-to-OpenGL ES wrapper, primarily intended for running Minecraft on Android.

## Building

### Android  

`./gradlew :ltw:assembleRelease`  

After completion, an AAR with native libraries will be available in `ltw/build/outputs/aar/ltw-release.aar`

### Linux

```
cd ltw/src/main/tinywrapper
mkdir build && cd build
cmake ..
make -j$(nproc)
```

The result library is `libltw.so`

## Usage

LTW wraps system `libEGL` thus you need to load it as a EGL library.  
A custom GLFW library might be needed for Minecraft. See: https://github.com/MojoLauncher/glfw.

## Used projects

[GLSL optimizer](https://github.com/aras-p/glsl-optimizer) (MIT license)
