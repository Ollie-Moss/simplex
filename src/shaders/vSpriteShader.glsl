#version 430 core

// vertex shader
layout (location = 0) in vec3 instancePos;     // per-instance
layout (location = 1) in vec2 instanceSize;    // per-instance
layout (location = 2) in vec4 instanceColor;   // per-instance

out vec2 texCoord;

uniform mat4 projection;

// Offsets for our vertices drawing this face
const vec3 facePositions[4] = vec3[4](
        vec3(0.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(1.0, 1.0, 0.0),
        vec3(1.0, 0.0, 0.0)
    );

int indices[6] = {
        0,
        1,
        2,
        3,
        0,
        2
    };

out vec4 vColor;

void main() {
    int currVertexID = gl_VertexID % 6;

    vec3 position = instancePos + facePositions[indices[currVertexID]] * vec3(instanceSize, 1.0);

    //gl_Position = projection * vec4(position, 1.0);
    gl_Position = vec4(position, 1.0);
    texCoord = vec2(facePositions[indices[currVertexID]].xy);
}
