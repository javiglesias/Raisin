#pragma once
#include "glm.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

unsigned int _CreateShaderData(char* _vertex_path, char* _fragment_path);
unsigned int _CreateShader(const char* _v_shader_code, const char* _f_shader_code);

// TODO: desligar el las matrices de transformacion del Modelo de Render.
struct Material
{
	enum PolygonMode
	{
		WIREFRAME	= 0x1B01,
		FILL		= 0x1B02
	};
	char mVertxName[128], mFragName[128];
	unsigned int mShaderId = -1;
	unsigned int mTextureId = -1;
	glm::vec3 mLightColor;
	PolygonMode mMode{ FILL };
	Material(){}
	Material(char* _vert, char* _frag, PolygonMode _Mode = FILL) {
		Init(_vert, _frag, _Mode);
	}
	void Init(char* _vert, char* _frag, PolygonMode _Mode = FILL)
	{
		printf("Loading shader: %s\n", _frag);
		mMode = _Mode;
		mShaderId = _CreateShaderData(_vert, _frag);
		printf("Shader loaded (id: %d)\n", mShaderId);
	}
};

inline Material* _CreateMaterial(const char* _mtlPath)
{
	// recibimos el path del fichero de material y lo parseamos para sacar la info.
	//[Shader]
	Material* mat = nullptr;
	FILE* mtl = fopen(_mtlPath, "r");
	if(!mtl)
		fprintf(stderr, "ERROR SHADER, no existe el MTL(%s)", _mtlPath);
	fseek(mtl, 0, SEEK_END);
	auto length = ftell(mtl);
	fseek(mtl, 0, SEEK_SET);
	char* data = (char*)malloc(length);
	fread(data, length, 1, mtl);
	char* it = data;
	char filename[64];
	char confTags[128];
	if (auto shaderName = strstr(it, "[Shader]"))
	{
		int count = 0;
		while(shaderName[count + 9] != '\n')
		{
			//[shader] = 8
			filename[count] = shaderName[count + 9];
			++count;
		}
		filename[count] = 0;
		if (auto conf = strstr(it, "[CONF]"))
		{
			count = 0;
			// [CONF] = 6
			while (conf[count + 7] != '\n')
			{
				//[shader] = 8
				confTags[count] = conf[count + 7];
				++count;
				if (conf[count + 7] == ' ')
				{// Vamos componiendo el nombre del shader necesario
					confTags[count] = '$';
					++count;
				}
			}
			confTags[count] = 0;
		}
	}
	else
	{
		fprintf(stderr, "ERROR SHADER, No existe la definico de Shader en el Mtl");
		exit(-9999);
	}
	//[CONF]
	char vert[128];
	sprintf(vert, "resources/Shaders/%s.vert", filename);
	char frag[128];
	sprintf(frag, "resources/Shaders/%s.frag$%s", filename, confTags);
	return new Material(vert, frag);
}

inline void _CompileShader(char* _path)
{
	// TODO compilacion de uber shader
	// #version 450 core
	int def = 0, count = 0, combinatory = 0;
	char filepath[128];
	strcpy(filepath, _path);

	std::ofstream v_uber_shader_file;
	std::ofstream f_uber_shader_file;
	std::string shaderCode;
	if (strstr(filepath, "$"))
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
			if (i >= (def + sizeof("#ifdef")))
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

inline unsigned int _CreateShaderData(char* _vertex_path, char* _fragment_path)
{
	std::string mVertShaderCode, mFragShaderCode;
	std::ifstream v_shader_file;
	std::ifstream f_shader_file;
	// Hay que compilar el Uber SHader antes de tenerlo disponible.
	FILE* f = fopen(_fragment_path, "r");
	if (!f) _CompileShader(_fragment_path);

	v_shader_file.open(_vertex_path);
	if (v_shader_file.is_open())
	{
		std::stringstream v_shader_stream;
		v_shader_stream << v_shader_file.rdbuf();
		v_shader_file.close();
		mVertShaderCode = v_shader_stream.str();
	}
	f_shader_file.open(_fragment_path);
	if(f_shader_file.is_open())
	{
		std::stringstream f_shader_stream;
		f_shader_stream << f_shader_file.rdbuf();
		f_shader_file.close();
		mFragShaderCode = f_shader_stream.str();
	}

	return _CreateShader(mVertShaderCode.c_str(), mFragShaderCode.c_str());
}