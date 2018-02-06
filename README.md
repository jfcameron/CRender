# CRender
![alt tag](http://jfcameron.github.io/Images/CRenderer_Windows_And_Android/Small.jpg "")

## Description:
![asdf](https://img.shields.io/badge/development%20status-closed-lightgrey.svg)
![asdf](https://img.shields.io/badge/platforms-win32%20|%20android-lightgrey.svg)

3D Renderer example program. Written strictly in ISO C90. Uses OpenGL ES 2.0. Builds for Win32 and Android via the NDK.


## Features:
Resource management for Textures, Shader programs, Vertex buffers.

Mat4x4 and Vector operations for calculating the MVP.

Textures and Shader programs can be loaded at runtime from the filesystem. Vertex buffers data must come from source code. Definitions for unit length quad and cube are provided.

## Building the project:
### Win32
* Navigate to _Windows, use the Visual Studio project files there

### Android
* Run the batch file at _Android/jni/ to build the native library
* Open the Eclipse project at _Android/, build APK

## Libraries used:
* [LinMath](https://github.com/datenwolf/linmath.h) - Mat4x4, Vector2-3 & Quaternion operations
* [GLFW](http://www.glfw.org/) - OpenGL context initialization
* [LodePNG](http://lodev.org/lodepng/) - PNG decoding library
* [UTHash](https://troydhanson.github.io/uthash/) - Hashmap functionality, used in Graphics.c to store textures, shaders, models and associate them with a name

## Tools used:
* The Win32 branch is setup for [Visual Studio](https://www.visualstudio.com/)
* The Android branch is setup for [Eclipse](https://eclipse.org/)
