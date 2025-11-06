#version 130 core

uniform sampler2D Texture;

in vec3 Normal;
in vec2 Texcoord;

out vec3 FragColor;

void main() {
	FragColor = Normal;
	//FragColor = texture(Texture, Texcoord).rgb;
}