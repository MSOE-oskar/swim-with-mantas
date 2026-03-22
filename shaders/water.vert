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
vec3 gerstnerWave(vec3 vertex, vec2 p_direction, float p_speed, float p_steepness, float p_amplitude, float p_wavelength){
    float frequency = 2.0 / p_wavelength;
    float cos_factor = cos(frequency * dot(p_direction, vertex.xz) + p_speed * time);

    float displaced_x = p_steepness * p_amplitude * p_direction.x * cos_factor;
	float displaced_z = p_steepness * p_amplitude * p_direction.y * cos_factor;
	float displaced_y = p_amplitude * sin(frequency * dot(p_direction, vertex.xz) + p_speed * time);
	return vec3(displaced_x, displaced_y, displaced_z);
}

vec3 gerstner(vec3 vertex) {
    vec3 result = waterHeight * vec3(0.0, 1.0, 0.0);
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

vec3 gerstner_normal(vec3 vertex, vec2 direction, float speed, float steepness, float amplitude, float wavelength) {
    float frequency = 2.0 / wavelength;
    float cosfactor = cos(frequency * dot(direction, vertex.xz) + speed * time);
	float sinfactor = sin(frequency * dot(direction, vertex.xz) + speed * time);
	float x_normal = direction.x * frequency * amplitude * cosfactor;
	float z_normal = direction.y * frequency * amplitude * cosfactor;
	float y_normal = (steepness/frequency) * frequency * amplitude * sinfactor;
	return vec3(x_normal, y_normal, z_normal);
}

vec3 waveNormal(vec3 vertex) {
    vec3 result = vec3(0.0);
    // GPUGems 1, Chapter 1, Equation 12
    for (int i = 0; i < octaves; ++i) {
        result += gerstner_normal(
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
    vec4 worldPos = vec4(gerstner(vec3(model * vec4(aPos, 1.0))), 1.0);
    gl_Position = projection * view * worldPos;

    FragPos = vec3(worldPos);
    Normal = waveNormal(vec3(worldPos));
    Color = aColor;
    TexCoord = aTexCoord;
}