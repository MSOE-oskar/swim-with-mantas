#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec4 Color;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D waterTexture;

void main()
{
    FragColor = texture(waterTexture, TexCoord);
}