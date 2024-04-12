#version 320 es

precision mediump float;

in vec3 texCoords;
out vec4 color;

uniform samplerCube cubemap;

void main() {
    color = texture(cubemap, texCoords);
}