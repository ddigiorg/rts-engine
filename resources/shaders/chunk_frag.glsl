#version 460 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D tileAtlas;

void main() {
    color = texture(tileAtlas, texCoords);
}