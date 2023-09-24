#pragma once
#include "glm.hpp"
// TODO: desligar el las matrices de transformacion del Modelo de Render.
struct Material
{
public:
	enum PolygonMode
	{
		WIREFRAME	= 0x1B01,
		FILL		= 0x1B02
	};
	bool mIsALight = false;
	unsigned int mShaderId = -1;
	glm::vec3 mLightColor;
	PolygonMode mMode;
	Material(unsigned int _ShaderId, bool _IsALight, glm::vec3 _LightColor = { 1.f, 1.f, 1.f }, PolygonMode _Mode = FILL)
	{
		mShaderId	= _ShaderId;
		mIsALight	= _IsALight;
		mLightColor = _LightColor;
		mMode		= _Mode;
	}
};