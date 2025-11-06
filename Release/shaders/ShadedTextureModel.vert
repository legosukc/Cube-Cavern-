#version 140 core

in vec3 aPos;
in vec3 aNormal;
in vec2 aTexcoord;

layout (std140) uniform RenderingMatrices {
	mat4 projection;
	mat4 view;
};
uniform mat4 model;

out vec3 Normal;
out vec3 FragPos;
out vec2 Texcoord;

void main() {
	
	vec4 ModelTransformation = model * vec4(aPos, 1.0);
	gl_Position = projection * ModelTransformation;
    
	Normal = transpose(inverse(mat3(model))) * aNormal;
    FragPos = ModelTransformation.xyz;
	Texcoord = aTexcoord;
}