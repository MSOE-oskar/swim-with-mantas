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
const float pi = 3.14159;

vec3 gerstnerWave(vec3 vertex, vec2 p_direction, float p_speed, float p_steepness, float p_amplitude, float p_wavelength){
    float displaced_x = vertex.x + (p_steepness/p_wavelength) * p_direction.x * cos(p_wavelength * dot(p_direction, vertex.xz) + p_speed * time);
	float displaced_z = vertex.z + (p_steepness/p_wavelength) * p_direction.y * cos(p_wavelength * dot(p_direction, vertex.xz) + p_speed * time);
	float displaced_y = vertex.y + p_amplitude * sin(p_wavelength * dot(p_direction, vertex.xz) + p_speed * time);
	return vec3(displaced_x, displaced_y, displaced_z);
}

// direction.y represents z axis (since it is 2D vec)
vec3 gerstner(vec3 vertex) {
    vec3 result = waterHeight * vec3(0.0, 1.0, 0.0);
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
    float cosfactor = cos(wavelength * dot(direction, vertex.xz + speed * time));
	float sinfactor = sin(wavelength * dot(direction, vertex.xz + speed * time));
	float x_normal = -direction.x * wavelength * amplitude * cosfactor;
	float z_normal = -direction.y * wavelength * amplitude * cosfactor;
	float y_normal = 1.0 - (steepness/wavelength) * wavelength * amplitude * sinfactor;
	return vec3(x_normal, y_normal, z_normal);
}

vec3 waveNormal(vec3 vertex) {
    vec3 result = vec3(0.0, 0.0, 0.0);
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