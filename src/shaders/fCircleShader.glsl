#version 330 core

in vec2 uv;
out vec4 FragColor;

uniform vec2 center;
uniform float radius;

void main()
{
    float dist = distance(uv, center);
    
    if (dist > radius)
        discard;

    FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
