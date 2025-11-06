#version 140 core

in vec2 aPos;
in vec2 aTexcoord;

noperspective out vec2 Texcoord;

layout (std140) uniform RenderingMatrices {
	mat4 projection;
	mat4 view;
};

uniform vec3 Position = vec3(0.0);

void main() {
	gl_Position = projection * view * vec4(Position, 1.0);
	gl_Position.xy += aPos;

	Texcoord = aTexcoord;
}