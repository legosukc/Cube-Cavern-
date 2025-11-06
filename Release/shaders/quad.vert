#version 130 core

in vec2 aPos;
in vec2 aTexcoord;

noperspective out vec2 Texcoord;

void main() {
	gl_Position = vec4(aPos, 1.0, 1.0);
	Texcoord = aTexcoord;
}