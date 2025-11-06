#version 130 core

uniform sampler2D RenderTexture;

noperspective in vec2 Texcoord;

out vec3 FragColor;

void main() {
	FragColor = texture(RenderTexture, Texcoord).rgb;
}