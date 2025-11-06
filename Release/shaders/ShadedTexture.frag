#version 130 core

uniform sampler2D Texture;

in vec3 Normal;
in vec3 FragPos;
in vec2 Texcoord;

out vec3 FragColor;

uniform vec3 LightDir = vec3(0.0);

const vec3 LightColor = vec3(1.0);

const float SpecularStrength = 0.5;
//const vec3 SpecularColor = vec3(1.0);

const float DiffuseStrength = 1.0;
//const vec3 DiffuseColor = vec3(1.0);

const float AmbientStrength = 0.1;
const vec3 AmbientColor = vec3(1.0);

void main() {

    vec3 ViewDir = normalize(-FragPos);

    vec3 Diffuse =  DiffuseStrength * LightColor * max(dot(Normal, ViewDir), 0.0);
    vec3 Specular = SpecularStrength * LightColor * pow(max(dot(ViewDir, reflect(-LightDir, Normal)), 0.0), 32.0);
    const vec3 Ambient = AmbientStrength * AmbientColor;

    FragColor = texture(Texture, Texcoord).rgb * (Diffuse + Specular + Ambient);
}