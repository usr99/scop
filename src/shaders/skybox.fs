#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube uCubemap;

void main()
{    
    FragColor = texture(uCubemap, TexCoords);
}