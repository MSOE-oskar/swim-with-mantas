#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec4 Color;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D sandTexture;

// using view position for lighting and fog calculations
uniform vec3 viewPos;
uniform vec3 fogColor;

void main()
{
    vec3 distance = viewPos - FragPos;
    float distanceLength = length(distance);
    // simple fog effect based on distance
    float fogFactor = clamp(1.0 - (distanceLength / 50.0), 0.0, 1.0);
    // apply fog color based on the fog factor
    vec3 finalColor = mix(fogColor, texture(sandTexture, TexCoord).rgb, fogFactor);

    FragColor = vec4(finalColor, 1.0);
}