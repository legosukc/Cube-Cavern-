#version 130 core

uniform sampler2D Texture;

noperspective in vec2 Texcoord;

out vec3 FragColor;

void main() {
	FragColor = texture(Texture, Texcoord).rgb;

	if (FragColor == vec3(1.0, 0.0, 0.0))
        discard;
}