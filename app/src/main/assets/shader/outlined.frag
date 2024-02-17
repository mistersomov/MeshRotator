#version 300 es

precision mediump float;

out vec4 color;

uniform float uTime;

vec3 palette(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263,0.416,0.557);

    return a + b * cos(6.28318 * (c * t + d));
}

void main() {
//    float d = length(uv);
//    vec3 col = palette(length(uv) * exp(-length(uv)));
//    vec3 finalColor = vec3(0.0);
//
//    d = sin(d * 8 + uTime) / 8;
//    d = abs(d);
//    d = pow(0.01 / d, 1.2);
//
//    finalColor += col * d;
    color = vec4(palette(1.5), 1.0);
}