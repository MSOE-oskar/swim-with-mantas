#version 330 core
out vec4 FragColor;

in mat3 TBN;
in vec4 Color;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D waterTexture;
uniform vec3 viewPos;

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
  	
    // diffuse 
    vec3 norm = normalize(TBN[2]); // Normal vector from TBN matrix
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