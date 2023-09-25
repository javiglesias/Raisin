#pragma once
#include "glm.hpp"
#include "Shader.h"

// TODO: desligar el las matrices de transformacion del Modelo de Render.
struct Material
{
public:
	enum PolygonMode
	{
		WIREFRAME	= 0x1B01,
		FILL		= 0x1B02
	};
	bool IS_A_LIGHT = false;
	unsigned int mShaderId = -1;
	glm::vec3 mLightColor;
	PolygonMode mMode;
	Shader* mShader;
	Material(const char* _vert, const char* _frag, bool _IsALight, glm::vec3 _LightColor = { 1.f, 1.f, 1.f }, PolygonMode _Mode = FILL)
	{
		char _fragName[256];
		char _vertName[256];
		sprintf(_vertName, "resources/Shaders/%s.vert", _vert);
		sprintf(_fragName, "resources/Shaders/%s.frag", _frag);
		mShader		= new Shader("resources/Shaders/basic_shader.vert", "resources/Shaders/basic_shader.frag");
		mShaderId	= mShader->id;
		IS_A_LIGHT	= _IsALight;
		mLightColor = _LightColor;
		mMode		= _Mode;
	}
};