cmake_minimum_required(VERSION 3.0)
project(rts-engine)

# setup SDL paths
set(SDL2_INCLUDE_DIR "D:/_projects/rts-engine/3rdparty/SDL/build/include")
set(SDL2_INCLUDE_CONFIG_DIR "D:/_projects/rts-engine/3rdparty/SDL/build/include-config-release")
set(SDL2_LIBRARY_DIR "D:/_projects/rts-engine/3rdparty/SDL/build/Release")

# # setup SDL TTF paths
# set(SDL2_TTF_INCLUDE_DIR "D:/_projects/rts-engine/3rdparty/SDL2_ttf/include")
# set(SDL2_TTF_LIBRARY_DIR "D:/_projects/rts-engine/3rdparty/SDL2_ttf/lib/x64")

# setup GLEW paths
set(GLEW_INCLUDE_DIR "D:/_projects/rts-engine/3rdparty/GLEW/include")
set(GLEW_LIBRARY_DIR "D:/_projects/rts-engine/3rdparty/GLEW/lib/Release/x64")

# setup OpenGL paths
set(OPENGL_INCLUDE_DIR "C:/Program Files (x86)/Windows Kits/10/Include/10.0.22000.0/um")
set(OPENGL_LIBRARY_DIR "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64")

# setup STB paths
set(STB_INCLUDE_DIR "D:/_projects/rts-engine/3rdparty/stb/")

# setup FastNoiseLite paths
set(FASTNOISELITE_INCLUDE_DIR "D:/_projects/rts-engine/3rdparty/FastNoiseLite/Cpp")

# setup FreeType2 paths
set(FREETYPE2_INCLUDE_DIR "D:/_projects/rts-engine/3rdparty/freetype/include")
set(FREETYPE2_LIBRARY_DIR "D:/_projects/rts-engine/3rdparty/freetype/build/Release")


include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_INCLUDE_CONFIG_DIR}
    # ${SDL2_TTF_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
    ${FASTNOISELITE_INCLUDE_DIR}
    ${FREETYPE2_INCLUDE_DIR}
)

add_executable(
    rts-engine
    source/main.cpp
)

target_link_libraries(
    rts-engine
    ${SDL2_LIBRARY_DIR}/SDL2.lib
    ${SDL2_LIBRARY_DIR}/SDL2main.lib
    # ${SDL2_TTF_LIBRARY_DIR}/SDL2_ttf.lib
    ${GLEW_LIBRARY_DIR}/glew32.lib
    ${OPENGL_LIBRARY_DIR}/OpenGL32.lib
    ${FREETYPE2_LIBRARY_DIR}/freetype.lib
)