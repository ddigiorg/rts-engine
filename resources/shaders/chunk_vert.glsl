#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;
out vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {

    gl_Position = projection * view * vec4(pos, 0.0f, 1.0f);
    texCoords = uv;
}