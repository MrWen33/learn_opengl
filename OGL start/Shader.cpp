#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<string>
#include<iostream>
using namespace std;
Shader::Shader(const GLchar * vertexPath, const GLchar * fragPath)
{
	string vertex_code;
	string frag_code;
	ifstream vertex_shader_file;
	ifstream frag_shader_file;
	vertex_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
	frag_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		vertex_shader_file.open(vertexPath);
		frag_shader_file.open(fragPath);
		stringstream vertex_shader_stream, frag_shader_stream;
		vertex_shader_stream << vertex_shader_file.rdbuf();
		frag_shader_stream << frag_shader_file.rdbuf();
		vertex_shader_file.close();
		frag_shader_file.close();
		vertex_code = vertex_shader_stream.str();
		frag_code = frag_shader_stream.str();
	}
	catch(ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	unsigned int vertex_id = get_vertex_shader(vertex_code.c_str());
	unsigned int frag_id = get_fragment_shader(frag_code.c_str());
	unsigned int shader_id = get_shader_program(vertex_id, frag_id);
	glDeleteShader(vertex_id);
	glDeleteShader(frag_id);
	ID = shader_id;
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string & name, bool value) const
{
	glUniform1i(
		glGetUniformLocation(
			ID, 
			name.c_str()
		), (int)value
	);
}

void Shader::setInt(const std::string & name, int value) const
{
	glUniform1i(
		glGetUniformLocation(
			ID,
			name.c_str()
		), value
	);
}

void Shader::setFloat(const std::string & name, float value) const
{
	glUniform1f(
		glGetUniformLocation(
			ID,
			name.c_str()
		), value
	);
}

void Shader::setMat4(const std::string & name, glm::mat4 mat4)
{
	glUniformMatrix4fv(
		glGetUniformLocation(
			ID,
			name.c_str()
		),
		1, GL_FALSE,
		glm::value_ptr(mat4)
	);
}

void Shader::setVec3(const std::string & name, glm::vec3 vec3)
{
	glUniform3fv(
		glGetUniformLocation(
			ID,
			name.c_str()
		), 1,
		glm::value_ptr(vec3)
	);
}

inline unsigned int Shader::get_vertex_shader(const char * source)
{
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &source, NULL);
	glCompileShader(vertexShader);
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "VERT ERROR\n" << infoLog << std::endl;
	}
	return vertexShader;
}

inline unsigned int Shader::get_fragment_shader(const char * source)
{
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &source, NULL);
	glCompileShader(fragmentShader);
	int success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "FRAG ERROR\n" << infoLog << std::endl;
	}
	return fragmentShader;
}

inline unsigned int Shader::get_shader_program(unsigned int vertexShader, unsigned int fragShader)
{
	unsigned shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "LINK ERROR\n" << infoLog << std::endl;
	}
	return shaderProgram;
}
