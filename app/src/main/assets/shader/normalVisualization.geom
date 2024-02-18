#version 320 es

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;
layout(std140) uniform Matrices {
    mat4 PROJECTION;
};

const float MAGNITUDE = 0.2;

in VS_OUT {
    vec3 normal;
} gs_in[];

void generate_line(int index) {
    gl_Position = PROJECTION * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = PROJECTION * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main() {
    generate_line(0);
    generate_line(1);
    generate_line(2);
}