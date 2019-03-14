#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D depthMap;

void main(){
	float depthValue = texture(depthMap, TexCoords).r;
	color = vec4(vec3(depthValue), 1.0);
	//color = vec4(1,1,1,1);
}