#version 320 es

layout(location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 VIEW;
uniform mat4 PROJECTION;

void main() {
    texCoords = aPos;
    gl_Position = (PROJECTION * VIEW * vec4(aPos, 1.0)).xyww;
}