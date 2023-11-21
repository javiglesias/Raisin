#pragma once
#include "glm.hpp"
#include "Shader.h"
#include "../Application.h"

struct Texture {
	int id = -1;
	std::string type = "";
	std::string path = "";
	int width = -1, heigth = -1, nr_channels = -1;
};

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
	Texture mTexture;
	Shader* mShader;
	Material(){}
	Material(char* _vert, char* _frag, PolygonMode _Mode = FILL) {
		Init(_vert, _frag, _Mode);
	}
	void Init(char* _vert, char* _frag, PolygonMode _Mode = FILL)
	{
		printf("Loading shader: %s\n", _frag);
		mMode = _Mode;
		mShader = new Shader(_vert, _frag);
		mShaderId = mShader->id;
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
