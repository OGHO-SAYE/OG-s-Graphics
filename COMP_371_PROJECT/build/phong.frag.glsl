#version 330 core
out vec4 FragColor;

in vec3 FragPos;    // Fragment position in world space
in vec3 Normal;     // Normal direction in world space

uniform vec3 lightPos;       // Position of the light source
uniform vec3 lightColor;     // Color of the light
uniform vec3 objectColor;    // Color of the object

void main()
{
    // Calculate lighting
    vec3 lightDir = normalize(lightPos - FragPos);
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
