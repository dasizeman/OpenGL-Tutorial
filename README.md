My work on the OpenGL tutorial from http://open.gl/.

This project also serves two other purposes, namely:
* To give me experience with Windows/Linux multi-platform builds
* To let me create an easy-start platform for OpenGL applications on both of
  these platforms.

Notes on building:
* I use CMake to maintain the build
** On Windows, the CMake GUI will let you generate VS project files.  Neat!  I'm
pretty sure only VS2015 will work, and I haven't tested other versions.
** On Linux, I recommend creating a build directory in the location of
CMakeLists.txt, changing to that directory  and doing cmake ./..  This will
generate the Makefile for you. Some notes about the Linux build:
** I have only tested building on Ubuntu 14.04.  On that platform, since I
already include the static libraries, the only extra dependency package I've
found that you need so far is libglu1-mesa-dev.  I will update as I find more.
Also please let me know if you build this on other Linux platforms, and how you
did it, or if you found any other dependencies that I missed for Ubuntu, so I
can update this info.

Notes on running:
* All shader (.glsl) files need to be copied to the same directory as the
  executable

Etc:
* Compiled library files for GLEW and GLFW3 are included for both platforms,
  free of charge
* Only a 32-bit version is available for now, meaning for:
** Windows: Only the x86 VS build configuration is supported
** Linux: You need the gcc-multilib compiler
