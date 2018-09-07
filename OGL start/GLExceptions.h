#pragma once
#include<string>
#include<exception>
using std::string;
struct VertexShaderCompileException : public std::exception
{
	const char * error_log;
	VertexShaderCompileException(const char* log) {
		error_log = log;
	}

	const char * what() const throw()
	{
		string error_info = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
		error_info += error_log;
		return error_info.c_str();
	}
};

struct FragmentShaderCompileException : public std::exception
{
	const char * error_log;
	FragmentShaderCompileException(const char* log) {
		error_log = log;
	}

	const char * what() const throw()
	{
		string error_info = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
		error_info += error_log;
		return error_info.c_str();
	}
};

struct LinkShaderException : public std::exception
{
	const char * error_log;
	LinkShaderException(const char* log) {
		error_log = log;
	}

	const char * what() const throw()
	{
		string error_info = "ERROR::SHADER::LINK::COMPILATION_FAILED\n";
		error_info += error_log;
		return error_info.c_str();
	}
};