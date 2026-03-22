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
uniform float steepness[8];
uniform float amplitude[8];
uniform float wavelength[8];
uniform float speed[8];
uniform vec2 direction[8];

out vec3 FragPos;
out vec3 Normal;
out vec4 Color;
out vec2 TexCoord;

// Credit: https://jayconrod.com/posts/34/water-simulation-in-glsl
// and: https://gameidea.org/2023/12/01/3d-ocean-shader-using-gerstner-waves/
// and: https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models
vec4 gerstnerWave(vec4 vertex, vec2 direction, float speed, float steepness, float amplitude, float wavelength){
    float frequency = 2.0 / wavelength;
    float cosFactor = cos(frequency * dot(direction, vertex.xz) + speed * time);

    float xDisplacement = steepness * amplitude * direction.x * cosFactor;
	float zDisplacement = steepness * amplitude * direction.y * cosFactor;
	float yDisplacement = amplitude * sin(frequency * dot(direction, vertex.xz) + speed * time);
	return vec4(xDisplacement, yDisplacement, zDisplacement, 0.0);
}

vec4 gerstner(vec4 vertex) {
    vec4 result = waterHeight * vec4(0.0, 1.0, 0.0, 0.0);
    // GPUGems 1, Chapter 1, Equation 9
    result += vertex;
    for(int i = 0; i < octaves; ++i) {
        result += gerstnerWave(
            vertex, 
            normalize(direction[i]), 
            speed[i], 
            steepness[i], 
            amplitude[i], 
            wavelength[i]
            );
    }
    return result;
}

vec3 gerstnerNormal(vec3 vertex, vec2 direction, float speed, float steepness, float amplitude, float wavelength) {
    float frequency = 2.0 / wavelength;
    float cosFactor = cos(frequency * dot(direction, vertex.xz) + speed * time);
	float sinFactor = sin(frequency * dot(direction, vertex.xz) + speed * time);
	float xNormal = direction.x * frequency * amplitude * cosFactor;
	float zNormal = direction.y * frequency * amplitude * cosFactor;
	float yNormal = (steepness/frequency) * frequency * amplitude * sinFactor;
	return vec3(xNormal, yNormal, zNormal);
}

vec3 waveNormal(vec3 vertex) {
    vec3 result = vec3(0.0);
    // GPUGems 1, Chapter 1, Equation 12
    for (int i = 0; i < octaves; ++i) {
        result += gerstnerNormal(
            vertex, 
            normalize(direction[i]), 
            speed[i], 
            steepness[i], 
            amplitude[i], 
            wavelength[i]
            );
    }
    result = vec3(-result.x, 1.0 - result.y, -result.z);
    return normalize(result);
}

void main()
{
    // Matrix Multiplication order is important! 
    // It goes in reverse order of how you want to apply them.
    // so fricken cool
    vec4 worldPos = gerstner(model * vec4(aPos, 1.0));
    gl_Position = projection * view * worldPos;

    FragPos = vec3(worldPos);
    Normal = waveNormal(vec3(worldPos));
    Color = aColor;
    TexCoord = aTexCoord;
}