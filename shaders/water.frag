#version 330 core
out vec4 FragColor;

in mat3 TBN;
in vec4 Color;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D waterNormalMap1;
uniform sampler2D waterNormalMap2;

uniform vec3 viewPos;
uniform float time;

struct Light {
    vec4 direction;
    vec3 color;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Light light;
uniform Material material;

void main()
{
    // ambient
    vec3 ambient = light.color * material.ambient;
  	
    
    // sample normal map to calculate normal
    mat2 rot = mat2(0, 1, -1, 0);
    vec2 scrolledTexCoord1 = rot * (TexCoord + vec2(time * 0.05, time * 0.05));
    vec2 scrolledTexCoord2 = rot * (TexCoord - vec2(time * 0.013, time * 0.07));
    vec3 norm = texture(waterNormalMap1, scrolledTexCoord1).rgb + texture(waterNormalMap2, scrolledTexCoord2).rgb;
    norm = norm * 2.0 - 1.0;   
    norm = normalize(TBN * norm);

    // diffuse
    vec3 lightDir = normalize(-light.direction.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.color * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result * Color.rgb, Color.a);
}