#pragma once

#include "../stb_image/stb_image.h"
#include "RenderAPI.h"

struct sCubemap
{
	unsigned char*	mTextureData;
	unsigned int	mTextureId = -1;
	unsigned int	mVAO = -1;
	unsigned int	mShaderID = -1;
	Shader			mShader;
	int mWidth		= 0;
	int mHeigth		= 0;
	int mNChannels	= 0;
	const char mTexPath[128] = "resources/textures/cubemaps/office.png";
	const char mCubePath[128] = "resources/models/BasicShapes/cube/Cube.obj";
	float mCubemapVertices[24] = {
		1.000000,	1.000000,-1.000000,
		1.000000,	-1.000000,-1.000000,
		1.000000,	1.000000,1.000000,
		1.000000,	-1.000000,1.000000,
		-1.000000,	1.000000,-1.000000,
		-1.000000,	-1.000000,-1.000000,
		-1.000000,	1.000000,1.000000,
		-1.000000,	-1.000000,1.000000
	};

	void LoadTexData()
	{
		mTextureData = stbi_load(mTexPath,
			&mWidth, &mHeigth, &mNChannels, 0);
		if(mTextureData)
		{
			printf("CubemapLoad: %p\n", mTextureData);
			mTextureId = _CreateTexture(mTextureData, mWidth, mHeigth);
			stbi_image_free(mTextureData);
			LoadVAOData();
		}
	}
	void LoadVAOData()
	{
		mVAO = _CreateVAOData(mCubemapVertices, 24);
		mShader.Init("resources/Shaders/cubemap.vert", "resources/Shaders/cubemap.frag");
		mShaderID = mShader.id;
	}
	void Draw(glm::mat4 _ModelMatrix, glm::mat4 _ViewMatrix, glm::mat4 _ProjectionMatrix, glm::vec3 _CameraPosition)
	{
		_DrawArrays(_ModelMatrix, _ViewMatrix, _ProjectionMatrix, _CameraPosition,
			mShaderID, mVAO, GL_TRIANGLES, 0, 24);
	}
};