#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
struct Material{
	sampler2D texture_diffuse0;
};

uniform Material material;

void main()
{    
	//FragColor = vec4(1.0);
    FragColor = texture(material.texture_diffuse0, TexCoords);
}
