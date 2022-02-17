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

float near = 0.1;
float far = 100.0;

float linearDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth  = linearDepth(gl_FragCoord.z) / far;

    FragColor = vec4(vec3(depth), 1.0);
}