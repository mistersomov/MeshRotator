#version 320 es

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTex;

out vec2 texCoords;

void main() {
    texCoords = aTex;
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
}