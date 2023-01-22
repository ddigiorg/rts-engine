cmake_minimum_required(VERSION 3.0)
project(rts-engine)

# setup SDL paths
set(SDL2_INCLUDE_DIR "D:/_projects/rts-engine/3rdparty/SDL/build/include")
set(SDL2_INCLUDE_CONFIG_DIR "D:/_projects/rts-engine/3rdparty/SDL/build/include-config-release")
set(SDL2_LIBRARY_DIR "D:/_projects/rts-engine/3rdparty/SDL/build/Release")

# setup GLEW paths
set(GLEW_INCLUDE_DIR "D:/_projects/rts-engine/3rdparty/GLEW/include")
set(GLEW_LIBRARY_DIR "D:/_projects/rts-engine/3rdparty/GLEW/lib/Release/x64")

# setup OpenGL paths
set(OPENGL_INCLUDE_DIR "C:/Program Files (x86)/Windows Kits/10/Include/10.0.22000.0/um")
set(OPENGL_LIBRARY_DIR "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64")

# setup STB paths
set(STB_INCLUDE_DIR "D:/_projects/rts-engine/3rdparty/stb/")

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_INCLUDE_CONFIG_DIR}
    ${GLEW_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
)

add_executable(
    rts-engine
    source/main.cpp
)

target_link_libraries(
    rts-engine
    ${SDL2_LIBRARY_DIR}/SDL2.lib
    ${SDL2_LIBRARY_DIR}/SDL2main.lib
    ${GLEW_LIBRARY_DIR}/glew32.lib
    ${OPENGL_LIBRARY_DIR}/OpenGL32.lib
)