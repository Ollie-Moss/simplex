#version 430 core

// vertex shader
layout(location = 0) in vec3 inPosition; 

out vec4 color;

uniform mat4 projection;
uniform vec4 inColor;

void main() {
    vec3 position = inPosition;

    gl_Position = projection * vec4(position, 1.0);
    color = inColor;
}
