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
out mat3 TBN;
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

vec3 gerstnerTangent(float cosFactor, float sinFactor, vec2 direction, float speed, float steepness, float amplitude, float frequency) {
    // GPUGems 1, Chapter 1, Equation 11
    float xTangent = steepness * direction.x * direction.y * frequency * amplitude * sinFactor;
    float zTangent = steepness * direction.y * direction.y * frequency * amplitude * sinFactor;
    float yTangent = direction.y * frequency * amplitude * cosFactor;
    return vec3(xTangent, yTangent, zTangent);
}

vec3 gerstnerBitangent(float cosFactor, float sinFactor, vec2 direction, float speed, float steepness, float amplitude, float frequency) {
    // GPUGems 1, Chapter 1, Equation 10
    float xBitangent = steepness * direction.x * direction.x * frequency * amplitude * sinFactor;
    float zBitangent = steepness * direction.x * direction.y * frequency * amplitude * sinFactor;
    float yBitangent = direction.x * frequency * amplitude * cosFactor;
    return vec3(xBitangent, yBitangent, zBitangent);
}

mat3 waveTBN(vec3 vertex) {
    vec3 T = vec3(0.0, 0.0, 0.0);
    vec3 B = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < octaves; ++i) {
        float frequency = 2.0 / wavelength[i];
        float factorInnards = frequency * dot(normalize(direction[i]), vertex.xz) + speed[i] * time;
        float cosFactor = cos(factorInnards);
        float sinFactor = sin(factorInnards);
        T += gerstnerTangent(cosFactor, sinFactor, normalize(direction[i]), speed[i], steepness[i], amplitude[i], frequency);
        B += gerstnerBitangent(cosFactor, sinFactor, normalize(direction[i]), speed[i], steepness[i], amplitude[i], frequency);
    }
    T = vec3(-T.x, T.y, 1 - T.z);
    B = vec3(1 - B.x, B.y, -B.z);
    vec3 N = cross(T, B);
    return mat3(T, B, N);
}

void main()
{
    // Matrix Multiplication order is important! 
    // It goes in reverse order of how you want to apply them.
    // so fricken cool
    vec4 worldPos = gerstner(model * vec4(aPos, 1.0));
    gl_Position = projection * view * worldPos;

    FragPos = vec3(worldPos);
    TBN = waveTBN(vec3(worldPos));
    Color = aColor;
    TexCoord = aTexCoord;
}