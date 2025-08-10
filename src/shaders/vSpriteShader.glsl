#version 430 core

struct RenderData {
    vec3 position;
    vec2 size;
    vec4 color;
};

// vertex shader
layout(location = 0) in vec3 inPosition; // per-instance
layout(location = 1) in vec2 inSize; // per-instance
layout(location = 2) in vec4 inColor; // per-instance
layout(location = 3) in vec3 inQuadVertex; // per-vertex

out vec2 texCoord;
out vec4 color;

uniform mat4 projection;

void main() {
    vec3 scale = vec3(inSize, 1.0);
    vec3 position = (inQuadVertex * scale) + inPosition;

    gl_Position = projection * vec4(position, 1.0);
    texCoord = inQuadVertex.xy;
    color = inColor;
}
