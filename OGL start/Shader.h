#pragma once
#include<glad\glad.h>
#include<glm\glm.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader
{
public:
	unsigned int ID;

	Shader(const GLchar* vertexPath, const GLchar* fragPath);

	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 mat4);
	void setVec3(const std::string& name, glm::vec3 vec3);
private:
	unsigned int get_vertex_shader(const char * source);

	unsigned int get_fragment_shader(const char * source);

	unsigned int get_shader_program(
		unsigned int vertexShader,
		unsigned int fragShader);
};