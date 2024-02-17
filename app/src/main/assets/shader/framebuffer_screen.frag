#version 300 es

precision mediump float;

in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

void main() {
    color = vec4(texture(screenTexture, TexCoords).rgb, 1.0);
}