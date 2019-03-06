#version 330 core

out vec4 fcolor;
uniform vec3 color;
uniform sampler2D depthMap;
in vec4 lightSpacePos;

void main(){
	float bias = 0.0005;
	vec3 fragPosDepth = lightSpacePos.xyz/lightSpacePos.w;
	fragPosDepth = fragPosDepth*0.5+0.5;
	if(texture(depthMap, fragPosDepth.xy).r+bias>fragPosDepth.z){
		fcolor = vec4(color, 1.0);
	}else{
		fcolor = vec4(0.1*color, 1.0);
	}
	//fcolor = vec4(color, 1.0);
}

