#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube cloudbox;

void main()
{    
    FragColor = texture(cloudbox, TexCoords);
}