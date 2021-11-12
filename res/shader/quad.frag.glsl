#version 460 core

in vec2 textureCoords;

out vec4 out_color;

uniform sampler2D imageTexture;

void main() {
    out_color = texture(imageTexture, textureCoords);
    
}