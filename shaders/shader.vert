#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Matrix Multiplication order is important! 
    // It goes in reverse order of how you want to apply them.
    // so fricken cool
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // set outputs
    TexCoord = aTexCoord;
}