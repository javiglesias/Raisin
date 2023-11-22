#pragma once
#include "Model.h"
#include "RenderAPI.h"
#include "Primitives/Cubemap.h" 

namespace RaisinEng
{
	inline unsigned char mFrameCPerf = 0;
	inline std::vector<Mesh> oMeshes;
	inline sCubemap mCubemap;
	struct RenderObj
	{
		int	iCurrentModels = 0;
		Material* mMat;
		Model mModels[256];
		void Draw();
		RenderObj(){}
		RenderObj(std::string path)
		{
			AddModel(path);
		}
		void AddModel(std::string path)
		{
			mModels[iCurrentModels] = Model(path, oMeshes);
			++iCurrentModels;
		}
	};
	inline bool		bMouseCaptured	= false;
	inline int		iCurrentObjs	= 0;
	inline float	fCameraSpeed	= .1f;
	inline float	fLastXPosition	= 0.f;
	inline float	fLastYPosition	= 0.f;
	inline float	fYaw = 0.f;
	inline float	fPitch = 0.f;
	inline glm::vec3 vCameraPosition(0.f);
	inline glm::vec3 vLightPosition(0.f);
	inline glm::vec3 vCameraForward(0.f, 0.f, 1.f);
	inline glm::vec3 vCameraUp(0.f, 1.f, 0.f);
	inline glm::vec3 vCameraRight(1.f, 0.f, 0.f);
	inline glm::vec3 vUp(0.f, 1.f, 0.f);
	inline glm::mat4 mModelMatrix{ 1.f };
	inline glm::mat4 mLightModelMatrix{ 1.f };
	inline glm::mat4 mViewMatrix{ 1.f };
	inline glm::mat4 mProjectionMatrix{ 1.f };
	inline RenderObj oObjsToDraw[256];
	inline float mPercentileFPS[255];

	void mouse_press_callback(int _button, int _action, int _mods);
	void _process_input(GLFWwindow* _window);
	void Editor_Init();
	void Editor_Loop();
	void Editor_Close();

	inline GLFWwindow* m_window = nullptr;
	inline bool Foolscreen = true;
	inline ax::NodeEditor::EditorContext* m_Context;

	inline void framebuffer_size_callback(GLFWwindow* _window, int _width, int _height)
	{
		glViewport(0, 0, _width, _height);	
	}
	void _Destroy();
	void Editor_Close();
	void _Loop();
	void _Init(int _Width = 800, int _Height = 600, const char* _AppName = "RaisinEngine");
};
