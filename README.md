# davinci
A lightweight graphics library (think about three.js as to webgl) in C++

## Prerequisites
* You need any form of C++11 compiler and build tool [CMake](https://cmake.org/download/)
* You need [glew](http://glew.sourceforge.net/) a cross-platform library to expose OpenGL core and extension functionality.
* You need [freeglut](http://freeglut.sourceforge.net/) to provide a simple cross-platform window system.
* You need [freetype](https://www.freetype.org/download.html) to produce high-quality output (glyph images) of most vector and bitmap font formats. (optional, see step 2.1 on how to toggle it.)

Depending on your Linux distribution you can install these dependencies
using `yum` or `apt-get`. Some of these packages might already be
installed or might have slightly different names.

Type the following to install the dependencies using `yum`:

    sudo yum install cmake.x86_64

Type the following to install the dependencies using `apt-get`:

    sudo apt-get install cmake-curses-gui

Under Mac OS X these dependencies can be installed using
[MacPorts](http://www.macports.org/):

    sudo port install cmake

## Compiling Source Code
Building from the source code through CMake is easy:

### 1.Create a build directory, and go into it
```
        mkdir davinci/build
        cd davinci/build
```
I do recommend having separate build directory from the source directory, so that you won't accidentally checked in the temporary files generated during the build process.   

### 2.1 Build through CMake CLI
```
        cmake ../path/to/davinci_source
```
In case cmake cannot find the dependencies, you can alway tell cmake where they can be found by defining the root of the library installation as follows
```
        cmake -DGLEW_ROOT_DIR="path/to/glew_root" 
              -DFREEGLUT_ROOT_DIR="path/to/freeglut_root"
              -DDAVANCI_ENABLE_TEXT_RENDERING="true"

              ../path/to/davinci_source
```
CMake would try it best to guess what is the most appropriate build system generator you need based on your machine OS, for example, Unix Makefile for linux OS, Visual Studio for Windows. If you don't like the default build system generator, you can overide it by passing additional cmake parameter `-G"generator name"`
For example, on Windows with Visual Studio 2013 x64:
```
       cmake -G"Visual Studio 12 2013 Win64"
```
on Linux:
```
       cmake -G"Unix Makefiles"
```
Type `cmake --help` for more options.

### 2.2 Build through CMake GUI  
Beside CLI tool, CMake also provides an user friendly GUI to configure your build.
The following screen shot is an example of how to run and configure the cmake through its GUI on Windows machine.
![CMakeGUI](/resource/cmake-gui.PNG?raw=true "Screen shot of cmake gui")
In case CMake cannot find the aforementioned dependencies, you can manually specify the root of the installation of each library on the UI just like you can define the cmake variable through its CLI in step 2.1.
Once the pathes are correctly set, hit `Configure`. If this is your first time to configure the project, you will be prompted to select appropriate `generator` for this project.
Click `Generate` to create makefile or Visual Studio sln file based on your selection in the last step.


### 3 Launch build system generator
Lastly, navigate to the build directory and build the project using
```
    make
```
or launch Visual Studio.

# License
MIT
