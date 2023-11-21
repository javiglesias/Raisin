#include "Shader.h"
#include "RenderAPI.h"
#include <fstream>
#include <sstream>
#include <iostream>


Shader::Shader(char* _vertex_path, char* _fragment_path)
{
	Init(_vertex_path, _fragment_path);
}

void Shader::Init(char* _vertex_path, char* _fragment_path)
{
	std::ifstream v_shader_file;
	std::ifstream f_shader_file;
	// Hay que compilar el Uber SHader antes de tenerlo disponible.
	FILE* f = fopen(_fragment_path, "r");
	if(!f) CompileShader(_fragment_path);

	if (mVertShaderCode == "")
	{
		v_shader_file.open(_vertex_path);
		if (v_shader_file.is_open())
		{
			std::stringstream v_shader_stream;
			v_shader_stream << v_shader_file.rdbuf();
			v_shader_file.close();
			mVertShaderCode = v_shader_stream.str();
		}
	}

	if (mFragShaderCode == "")
	{
		f_shader_file.open(_fragment_path);
		std::stringstream f_shader_stream;
		f_shader_stream << f_shader_file.rdbuf();
		f_shader_file.close();
		mFragShaderCode = f_shader_stream.str();
	}

	id = _CreateShader(mVertShaderCode.c_str(), mFragShaderCode.c_str());
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
void Shader::CompileShader(char* _path)
{
	// TODO compilacion de uber shader
	// #version 450 core
	int def = 0, count = 0, combinatory = 0;
	char filepath[128];
	strcpy(filepath, _path);

	std::ofstream v_uber_shader_file;
	std::ofstream f_uber_shader_file;
	std::string shaderCode;
	if(strstr(filepath, "$"))
	{
		auto path = strtok(filepath, "$");
		std::ifstream shaderFile;
		shaderFile.open(path);
		std::stringstream shaderBuffer;
		shaderBuffer << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderBuffer.str();
	}
	//auto _include = shaderCode.find("#include ", _include);

	def = shaderCode.find("#ifdef ", def);
	std::string _uberFrag;
	char c[16][256];
	// Analizamos toda la combinatoria de casos
	while (def != -1)
	{
		int j = 0;
		for (size_t i = def; i < shaderCode.size(); ++i)
		{
			if (shaderCode[i] == '\t' || shaderCode[i] == '\n') break;
			if(i >= (def + sizeof("#ifdef")))
			{
				c[combinatory][j] = shaderCode[i];
				++j;
			}
		}
		c[combinatory][j] = '\0';
		def = shaderCode.find("#ifdef ", def + 1);
		++combinatory;
		++count;
	}
	// Generamos toda la combinatoria y la guardamos en fichero.
	for (size_t i = 0; i < combinatory; i++)
	{
		auto definesWildCard = shaderCode.find("//DEFINES");
		_uberFrag = "#define ";
		_uberFrag += c[i];
		_uberFrag += '\n\t';
		shaderCode.insert(definesWildCard, _uberFrag);

		std::string filename = filepath;
		filename += "$";
		filename += c[i];
		f_uber_shader_file.open(filename);
		f_uber_shader_file.write(shaderCode.c_str(), shaderCode.size());
		f_uber_shader_file.close();
	}
	_uberFrag = "";
	for (size_t i = 0; i < combinatory; i++)
	{
		// CREAMOS TODA LA COMBINATORIA DE SHADERS
		for (size_t t = i + 1; t < combinatory; t++)
		{
			auto definesWildCard = shaderCode.find("//DEFINES");
			_uberFrag = "#define ";
			_uberFrag += c[i];
			_uberFrag += '\n\t';
			_uberFrag += "#define ";
			_uberFrag += c[t];
			_uberFrag += '\n\t';
			shaderCode.insert(definesWildCard, _uberFrag);

			std::string filename = filepath;
			filename += "$";
			filename += c[i];
			filename += "$";
			filename += c[t];
			f_uber_shader_file.open(filename);
			f_uber_shader_file.write(shaderCode.c_str(), shaderCode.size());
			f_uber_shader_file.close();
		}
	}
}