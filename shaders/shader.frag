#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D sandTexture;

void main()
{
    // sample fragment color from texture, based on texture coordinates
    FragColor = texture(sandTexture, TexCoord);
}