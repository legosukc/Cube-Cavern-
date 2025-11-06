#version 140 core

in vec3 aPos;
in vec3 aNormal;
in vec2 aTexcoord;

layout (std140) uniform RenderingMatrices {
	mat4 projection;
	mat4 view;
};

out vec3 Normal;
out vec2 Texcoord;

void main() {
	gl_Position = projection * view * vec4(aPos / 10.0, 1.0);

	Normal = aNormal;
	Texcoord = aTexcoord;
}