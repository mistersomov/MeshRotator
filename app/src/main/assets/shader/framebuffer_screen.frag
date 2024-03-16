#version 320 es

precision mediump float;

in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

const float OFFSET = 1.0 / 300.0;
const int KERNEL_SIZE = 9;

vec2[KERNEL_SIZE] get_offsets();

void main() {
    FragColor = vec4(texture(screenTexture, texCoords).rgb, 1.0);
}

vec2[KERNEL_SIZE] get_offsets() {
    vec2 offsets[KERNEL_SIZE] = vec2[](
        vec2(-OFFSET, OFFSET), // top-left
        vec2(0.0f, OFFSET), // top-center
        vec2(OFFSET, OFFSET), // top-right
        vec2(-OFFSET, 0.0f), // center-left
        vec2(0.0f, 0.0f), // center-center
        vec2(OFFSET, 0.0f), // center-right
        vec2(-OFFSET, -OFFSET), // bottom-left
        vec2(0.0f, -OFFSET), // bottom-center
        vec2(OFFSET, -OFFSET)  // bottom-right
    );
    return offsets;
}