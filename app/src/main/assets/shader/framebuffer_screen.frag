#version 320 es

precision mediump float;

in vec2 texCoords;
out vec4 color;

uniform sampler2D screenTexture;

void main() {
    color = vec4(texture(screenTexture, texCoords).rgb, 1.0);
}