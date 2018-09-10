#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 frag_pos;
in vec3 normal;

struct Material{
	sampler2D texture_diffuse0;
};

struct Light{
	vec3 pos; 
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
};

uniform Material material;
uniform Light light;
uniform vec3 camPos;

void main()
{    
	vec3 light_dir = normalize(frag_pos - light.pos);
	vec3 view_dir = normalize(frag_pos - camPos);
	
	vec4 color = texture(material.texture_diffuse0, TexCoords);

	float cosangle = max(dot(-light_dir, normal), 0.0);
	vec4 diff_light = vec4(light.diffuse, 1.0) * color * cosangle;
    
	vec4 ambient_light = vec4(light.ambient, 1.0) * color;

	vec3 reflect_dir = reflect(light_dir, normal);
	vec4 spec_light = pow(max(dot(reflect_dir, -view_dir), 0.0), 32) * vec4(light.specular, 1.0) * color;
	FragColor = ambient_light + diff_light + spec_light;
}
