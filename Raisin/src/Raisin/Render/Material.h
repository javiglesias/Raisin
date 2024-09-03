#pragma once
#include "Texture.h"
unsigned int _CreateShaderData(char* _vertex_path, char* _fragment_path);

struct Material
{
	enum STATE : char
	{
		INVALID,
		INITIALIZED
	};
	STATE mState {INVALID};
	char mShaderPath[32] = "resources/Shaders/basic_shader\0";
	unsigned int mShaderId = -1;
	Texture* mTextureAmbient;
	 Material(){}
	void DefaultInit();
	Material(char* _vert, char* _frag);
	void Init(char* _vert, char* _frag);
};