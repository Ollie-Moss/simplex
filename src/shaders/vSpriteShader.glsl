#version 430 core

// vertex shader
layout(location = 0) in vec3 inPosition; // per-instance
layout(location = 1) in vec2 inSize; // per-instance
layout(location = 2) in vec4 inColor; // per-instance
layout(location = 3) in vec2 inTexCoord; // per-vertex

out vec2 texCoord;
out vec4 color;

uniform mat4 projection;

// Offsets for our vertices drawing this face
const vec3 facePositions[4] = vec3[4](
        vec3(0.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(1.0, 1.0, 0.0),
        vec3(1.0, 0.0, 0.0)
    );

// Winding order to access the face positions
int indices[6] = {
        0,
        1,
        2,
        3,
        0,
        2
    };

void main() {
    int index = gl_VertexID / 6;
    int currVertexID = gl_VertexID % 6;

    vec3 quadVertex = facePositions[indices[currVertexID]];

    vec3 scale = vec3(inSize, 1.0);
    vec3 position = (quadVertex * scale) + inPosition;

    gl_Position = projection * vec4(position, 1.0);
    texCoord = inTexCoord;
    color = inColor;
}
