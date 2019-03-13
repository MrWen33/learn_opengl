#include "utils.h"
#include "ImgLoader.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

using std::cout;
using std::endl;

unsigned int loadMaterial(std::string path)
{
	ImgLoader img(path);
	if (img.width == 0 && img.height == 0) {
		cout << "Image(path=" << path << ") not found." << endl;
	}
	unsigned int ID;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		img.width, img.height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	img.free();
	return ID;
}
