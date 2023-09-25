#include "Shader.h"
#include "RenderAPI.h"
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
	CompileShader(vertex_path, fragment_path,vertex_code, fragment_code);
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
void Shader::CompileShader(const char* vertex_path, const char* fragment_path, std::string _vertex, std::string _fragment)
{
	// TODO compilacion de uber shader
	int def = 0, count = 0, combinatory = 0;
	std::ofstream v_uber_shader_file;
	std::ofstream f_uber_shader_file;
	def = _fragment.find("#ifdef ", def);
	std::string _uberFrag;
	char c[16][256];
	// Analizamos toda la combinatoria de casos
	while (def != -1)
	{
		int j = 0;
		for (size_t i = def; i < _fragment.size(); ++i)
		{
			if (_fragment[i] == '\t' || _fragment[i] == '\n') break;
			if(i >= (def + sizeof("#ifdef")))
			{
				c[combinatory][j] = _fragment[i];
				++j;
			}
		}
		c[combinatory][j] = '\0';
		def = _fragment.find("#ifdef ", def + 1);
		++combinatory;
		++count;
	}
	// Generamos toda la combinatoria y la guardamos en fichero.
	for (size_t i = 0; i < combinatory; i++)
	{
		_uberFrag = "#define ";
		_uberFrag += c[i];
		_uberFrag += '\n\t';
		_uberFrag += _fragment;

		std::string filename = fragment_path;
		filename += "$";
		filename += c[i];
		f_uber_shader_file.open(filename);
		f_uber_shader_file.write(_uberFrag.c_str(), _uberFrag.size());
		f_uber_shader_file.close();
	}
	_uberFrag = "";
	for (size_t i = 0; i < combinatory; i++)
	{
		// CREAMOS TODA LA COMBINATORIA DE SHADERS
		for (size_t t = i + 1; t < combinatory; t++)
		{
			_uberFrag = "#define ";
			_uberFrag += c[i];
			_uberFrag += '\n\t';
			_uberFrag += "#define ";
			_uberFrag += c[t];
			_uberFrag += '\n\t';
			_uberFrag += _fragment;

			std::string filename = fragment_path;
			filename += "$";
			filename += c[i];
			filename += "$";
			filename += c[t];
			f_uber_shader_file.open(filename);
			f_uber_shader_file.write(_uberFrag.c_str(), _uberFrag.size());
			f_uber_shader_file.close();
		}
	}
}