#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;
	vec3 direction;
	float cutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

void main()
{
	vec3 material_diffuse = vec3(texture(material.diffuse, TexCoord));
	vec3 material_spec =vec3(texture(material.specular, TexCoord));
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	if (theta > light.cutOff)
	{

		float diff = max(dot(norm, lightDir), 0.0);

		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec3 specular = material_spec * spec * light.specular;

		vec3 diffuse = diff * material_diffuse * light.diffuse;

		vec3 ambient = material_diffuse * light.ambient;

		vec3 result = (ambient + diffuse + specular);
		FragColor = vec4(result, 1.0);
	} else {
		FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoord)), 1.0);
	}
}
