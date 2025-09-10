#version 330 core
out vec4 FragColor;
//normal is allready normalized in vert shader
in vec3 Normal;
in vec3 FragPos;
in vec3 BasePos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 foamColor = vec3 (0.9, 1, 0.9);
float maxHeightOffset = 1.5;
float heightMult = 0.001;
float specularStrength = 0.5;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(Normal,lightDir),0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);  

    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    float maxHeight = BasePos.y + maxHeightOffset;

    float waveHeight = maxHeight - FragPos.y;

    float fixedWaveHeight = ((waveHeight - BasePos.y) * 100) / (maxHeight - BasePos.y);
    
    vec3 foamedObjColor = mix(foamColor,objectColor,fixedWaveHeight * heightMult);
    

    vec3 result = (ambient + diffuse + specular) * foamedObjColor;
    FragColor = vec4(result, 1.0);
}  