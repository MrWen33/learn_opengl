#version 330 core

in vec2 texCoord;
uniform sampler2D texture1;

out vec4 fragColor;

void main(){
	//fragColor = vec4(1,1,1,1);
	fragColor = texture(texture1, texCoord);
}