#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>


Shader::Shader(const char* vertex_path, const char* fragment_path, const char* _name)
{
	std::string vertex_code;
	std::string fragment_code;
	std::ifstream v_shader_file;
	std::ifstream f_shader_file;

	v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	name = _name;
	try
	{
		v_shader_file.open(vertex_path);
		f_shader_file.open(fragment_path);
		std::stringstream v_shader_stream, f_shader_stream;
		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();
		v_shader_file.close();
		f_shader_file.close();
		vertex_code = v_shader_stream.str();
		fragment_code = f_shader_stream.str();
	} catch(std::ifstream::failure e)
	{
		std::cerr << e.what();
		return;
	}
	const char* v_shader_code = vertex_code.c_str();
	const char* f_shader_code = fragment_code.c_str();
	id = _CreateShader(v_shader_code, f_shader_code);
}

void Shader::use()
{
	_UseShader(id);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
	//glUniform3fv(glGetUniformLocation(id, name.c_str()), value);
}