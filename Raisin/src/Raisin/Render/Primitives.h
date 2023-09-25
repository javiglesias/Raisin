#pragma once
#include "RenderAPI.h"
#define PI 3.14
#define PIPI 2*3.14

struct sSphere
{
private:
	unsigned int VBO, VAO;
	// circle
	float mVertices[48];
	float mRadio = 100, mCenterX{0.f}, mCenterY{0.f};
	float mAngle = 0.3927;
	glm::mat4 mModelMatrix{1.f};
	glm::mat4 mViewMatrix{ 1.f };
	glm::mat4 mProjectionMatrix{ 1.f };
	glm::vec3 mCameraPosition{1.f};
	Shader mShader;
public:
	void _calculateSphere()
	{
		for (int i = 0; i < 48; i += 3)
		{
			mVertices[i] = (mCenterX + mRadio * cos(i * mAngle)) / 800.f;// x = cx + r * cos(a)
			mVertices[i + 1] = (mCenterY + mRadio * sin(i * mAngle)) / 600.f; //y = cy + r * sin(a)
			mVertices[i + 2] = 0.f;
		}
	}
	sSphere() : mShader("resources/Shaders/basic_shape_shader.vert", "resources/Shaders/basic_shape_shader.frag")
	{
		VAO = _Create2DSphere(mRadio, mCenterX, mCenterY, mAngle);

	}
	void _draw(glm::mat4 _ModelMatrix, glm::mat4 _ViewMatrix, glm::mat4 _ProjectionMatrix, glm::vec3 _CameraPosition)
	{
		glPolygonMode(GL_FRONT, GL_LINE);
		_DrawArrays(_ModelMatrix, _ViewMatrix, _ProjectionMatrix, _CameraPosition, mShader.id, 
			VAO, GL_POINTS, 0, 48);
	}
};