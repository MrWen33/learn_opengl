#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D texture1;
uniform vec3 camPos;
uniform vec3 lightPos;

out vec4 fragColor;

void main(){
	float ambient = 0.1f;
	float shininess = 32;
	
	vec3 lightDir = normalize(lightPos-fragPos);
	vec3 viewDir = normalize(camPos-fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	float diffuse = max(dot(normal, lightDir), 0.0);
	float spec = pow(max(dot(reflectDir, viewDir), 0), shininess);
	//float spec = 0;
	float brightness = ambient+spec+diffuse;
	fragColor = brightness*texture(texture1, texCoord);
}