#version 330 core

out vec4 FragColor;
in vec2 texCoord;
in vec4 color;

uniform sampler2D ourTexture;
uniform int useTexture;


void main() {
    vec4 texColor = texture(ourTexture, texCoord);
    if (texColor.a > 0.0) {
        FragColor = mix(texColor, color, color.a);
    } else {
        FragColor = texColor;
    }

    if(useTexture == 0){
        FragColor = color;
    }
}
