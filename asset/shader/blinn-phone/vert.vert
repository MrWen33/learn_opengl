#version 330 core

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_texCoord;
layout(location=2) in vec3 in_normal;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	texCoord = in_texCoord;
	normal = normalize(vec3(transpose(inverse(model))*vec4(in_normal, 0)));
	fragPos = vec3(model*vec4(in_position, 1.0f));
	gl_Position = projection*view*model*vec4(in_position, 1.0f);
}