#version 330 core

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_fragPos;

out vec2 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	fragPos = in_fragPos;
	gl_Position = projection*view*model*vec4(in_position, 1.0f);
}