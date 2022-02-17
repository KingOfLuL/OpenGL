#version 460 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission; 
    float shininess;
};

out vec4 FragColor;

in vec2 TexCoords;

uniform Material material;

void main()
{
    vec4 texCol = texture(material.diffuse, TexCoords);
    FragColor = texCol;
}