#pragma once
#include "glm.hpp"
#include "Shader.h"

struct Texture {
	int id = -1;
	std::string type = "";
	std::string path = "";
	int width = -1, heigth = -1, nr_channels = -1;
};

// TODO: desligar el las matrices de transformacion del Modelo de Render.
struct Material
{
public:
	enum PolygonMode
	{
		WIREFRAME	= 0x1B01,
		FILL		= 0x1B02
	};
	bool IS_LIGHT_AFFECTED = false, HAS_POINT_LIGHTS = false;
	char mVertxName[128], mFragName[128];
	char mUberVertxName[128], mUberFragName[128];
	unsigned int mShaderId = -1;
	glm::vec3 mLightColor;
	PolygonMode mMode{ FILL };
	Texture mTexture;
	Shader* mShader;
	Material(const char* _vert, const char* _frag, bool _IsLightAffected, glm::vec3 _LightColor = { 1.f, 1.f, 1.f }, PolygonMode _Mode = WIREFRAME)
	{
		IS_LIGHT_AFFECTED = _IsLightAffected;
		mLightColor = _LightColor;
		mMode		= _Mode;
		sprintf(mVertxName, "resources/Shaders/%s.vert", _vert);
		sprintf(mFragName, "resources/Shaders/%s.frag", _frag);
		sprintf(mUberFragName, "%s", mFragName);
		sprintf(mUberVertxName, "%s", mVertxName);
		GenerateUberName();
		mShader		= new Shader(mUberVertxName, mUberFragName);
		mShaderId	= mShader->id;
	}
	void GenerateUberName()
	{
		if(IS_LIGHT_AFFECTED)
			sprintf(mUberFragName, "%s$IS_LIGHT_AFFECTED", mFragName);
		if(HAS_POINT_LIGHTS)
			sprintf(mUberFragName, "%s$HAS_POINT_LIGHTS", mFragName);
	}
};