# Real Time Strategy (RTS) Engine

A SDL2/OpenGL engine and RTS game I am making for fun.  The goal is to build an open-world procedurally generated map like Minecraft and blend some elements from my favorite childhood games like Warcraft, Shining Force, Age of Empires, Heros of Might and Magic, Fire Emblum, etc.

## Dependencies

- A C/C++ Compiler of your choice:
   - Linux: [gcc/g++](https://gcc.gnu.org/)
   - Windows: [MSVC](https://visualstudio.microsoft.com/vs/)
   - MacOS: [Clang](https://clang.llvm.org/)
- [Git](https://git-scm.com/)
- [CMake](https://cmake.org/)
- [OpenGL](https://www.opengl.org/)
- [SDL2](https://www.libsdl.org/)
- [GLEW](https://glew.sourceforge.net/)
- [STB](https://github.com/nothings/stb)

## Windows Install

NOTE: I use git bash as my terminal which gives me basic linux-like commands.

Pull this repo from github and navigate into it.

```
git pull
cd
```

Create a `3rdparty/` directory:

```
mkdir 3rdparty
```

**Install CMake**

Navigate to <https://cmake.org/download/> and download the `Windows x64 Installer`

Run it to install CMake

**Install MSVC and OpenGL**

Download [MSVC Community](https://visualstudio.microsoft.com/vs/community/)

During the installation process under the `Workloads` tab be sure to select `Game development with C++`

Modify OpenGL path variables in `CMakeLists.txt`, e.g.

```
# setup OpenGL paths
set(OPENGL_INCLUDE_DIR "C:/Program Files (x86)/Windows Kits/10/Include/10.0.22000.0/um")
set(OPENGL_LIBRARY_DIR "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64")
```

**Install SDL2**

In a terminal use these commands

```
cd 3rdparty
git clone https://github.com/libsdl-org/SDL.git
cd SDL
git checkout release-2.26.x
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cmake install .


cd include
mkdir SDL2
cp -r * SDL2
```

Modify SDL path variables in `CMakeLists.txt`, e.g.

```
# setup SDL paths
set(SDL2_INCLUDE_DIR "D:/_projects/rts-engine/3rdparty/SDL/build/include")
set(SDL2_LIBRARY_DIR "D:/_projects/rts-engine/3rdparty/SDL/build/Release")
```

**Install GLEW**

Download the [GLEW Windows 32-bit and 64bit binaries](https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download)

Create a `GLEW/` directory in `3rdparty`

Copy the contents of `glew-2.2.0-win32.zip` to that newly created `GLEW` directory

Modify GLEW path variables in `CMakeLists.txt`, e.g.

```
# setup GLEW paths
set(GLEW_INCLUDE_DIR "D:/_projects/rts-engine/3rdparty/GLEW/include")
set(GLEW_LIBRARY_DIR "D:/_projects/rts-engine/3rdparty/GLEW/lib/Release/x64")
```

**Install STB**

In a terminal use these commands

```
cd 3rdparty
git clone https://github.com/nothings/stb.git
```

Modify STB path variables in `CMakeLists.txt`, e.g.

```
# setup STB paths
set(STB_INCLUDE_DIR "D:/_projects/rts-engine/3rdparty/stb/")
```

## Usage

Compile the project.

``
cd rts-engine
sh build.sh
``
