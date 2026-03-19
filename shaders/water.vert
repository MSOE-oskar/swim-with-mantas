#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// for water
uniform float waterHeight;
uniform float time;
uniform int octaves;
uniform float amplitude[8];
uniform float wavelength[8];
uniform float speed[8];
uniform vec2 direction[8];

out vec3 FragPos;
out vec3 Normal;
out vec4 Color;
out vec2 TexCoord;

// Credit: https://jayconrod.com/posts/34/water-simulation-in-glsl
const float pi = 3.14159;

float wave(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    return amplitude[i] * sin(theta * frequency + time * phase);
}

float waveHeight(float x, float y) {
    float height = 0.0;
    for (int i = 0; i < octaves; ++i)
        height += waterHeight + wave(i, x, y);
    return height;
}

float dWavedx(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].x * frequency;
    return A * cos(theta * frequency + time * phase);
}

float dWavedy(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].y * frequency;
    return A * cos(theta * frequency + time * phase);
}

vec3 waveNormal(float x, float y) {
    float dx = 0.0;
    float dy = 0.0;
    for (int i = 0; i < octaves; ++i) {
        dx += dWavedx(i, x, y);
        dy += dWavedy(i, x, y);
    }
    vec3 n = vec3(-dx, -dy, 1.0);
    return normalize(n);
}

void main()
{
    // Matrix Multiplication order is important! 
    // It goes in reverse order of how you want to apply them.
    // so fricken cool
    vec4 worldPos = model * vec4(aPos, 1.0);
    worldPos.y = waveHeight(worldPos.x, worldPos.z);
    gl_Position = projection * view * worldPos;

    FragPos = vec3(worldPos);
    Normal = waveNormal(worldPos.x, worldPos.z);
    Color = aColor;
    TexCoord = aTexCoord;
}