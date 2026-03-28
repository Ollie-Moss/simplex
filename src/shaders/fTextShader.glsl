#version 330 core
in vec2 TexCoords;
in vec2 FragPos;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

uniform vec4 clipRect;

void main()
{
    float cx = clipRect.x;
    float cy = clipRect.y;
    float cw = clipRect.x + clipRect.z;
    float ch = clipRect.y + clipRect.w;


    if (FragPos.x < cx || FragPos.x > cw ||
            FragPos.y < cy || FragPos.y > ch)
        discard;

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
