#include "RaiEng.h"

#include "../Input/Input.h"
#include "../Sound/Sound.h"
#include "Primitives/Primitives.h"

// Dedicated GPU for laptops with 2 https://stackoverflow.com/questions/16823372/forcing-machine-to-use-dedicated-graphics-card
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

void _mouse_press_callback(GLFWwindow* _window, int x, int y, int button)
{
	if (button == 0 && !RaisinEng::bMouseCaptured)
		RaisinEng::bMouseCaptured = true;
	else if(button != 1 && RaisinEng::bMouseCaptured)
		RaisinEng::bMouseCaptured = false;
}
void mouse_movement_callback(GLFWwindow* _window, double x, double y)
{
	float x_offset = (x - RaisinEng::fLastXPosition);
	float y_offset = (RaisinEng::fLastYPosition - y);
	float senseo = 0.1f;
	RaisinEng::fLastXPosition = x;
	RaisinEng::fLastYPosition = y;
	if(RaisinEng::bMouseCaptured)
	{
		x_offset *= senseo;
		y_offset *= senseo;
		RaisinEng::fYaw += x_offset;
		RaisinEng::fPitch += y_offset;
		// CONSTRAINTS
		if (RaisinEng::fPitch > 89.0f)  RaisinEng::fPitch = 89.0f;
		if (RaisinEng::fPitch < -89.0f) RaisinEng::fPitch = -89.0f;
		glm::vec3 camera_direction;
		camera_direction.x = cos(glm::radians(RaisinEng::fYaw)) * cos(glm::radians(RaisinEng::fPitch));
		camera_direction.y = sin(glm::radians(RaisinEng::fPitch));
		camera_direction.z = sin(glm::radians(RaisinEng::fYaw)) * cos(glm::radians(RaisinEng::fPitch));
		RaisinEng::vCameraForward = glm::normalize(camera_direction);
		RaisinEng::fLastXPosition = x;
		RaisinEng::fLastYPosition = y;
	}
}

void RaisinEng::_Init(int _Width, int _Height, const char* _AppName)
{
	m_window = &_CreateWindow("Raisin", 800, 600);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(m_window, _mouse_press_callback);
	glfwSetCursorPosCallback(m_window, mouse_movement_callback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#ifdef _OPENGL
	Editor_Init();
#endif
	/*
	 *	@param[in] window The window that received the event.
	 *  @param[in] key The [keyboard key](@ref keys) that was pressed or released.
	 *  @param[in] scancode The system-specific scancode of the key.
	 *  @param[in] action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.  Future
	 *  releases may add more actions.
	 *  @param[in] mods Bit field describing which [modifier keys](@ref mods) were
	 *  held down.
	 */
	glfwSetKeyCallback(m_window, [](GLFWwindow* _window, int _key, int _scancode, int _action, int)
	{
		if(_key == GLFW_KEY_W && _action == GLFW_PRESS)
			RaisinEng::vCameraPosition += RaisinEng::fCameraSpeed * RaisinEng::vCameraForward;
		if (_key == GLFW_KEY_A && _action == GLFW_PRESS)
			RaisinEng::vCameraPosition += glm::normalize(glm::cross(
				RaisinEng::vCameraUp, RaisinEng::vCameraForward)) * RaisinEng::fCameraSpeed;
		if (_key == GLFW_KEY_S && _action == GLFW_PRESS)
			RaisinEng::vCameraPosition -= RaisinEng::fCameraSpeed * RaisinEng::vCameraForward;
		if (_key == GLFW_KEY_D && _action == GLFW_PRESS)
			RaisinEng::vCameraPosition -= glm::normalize(glm::cross(
				RaisinEng::vCameraUp, RaisinEng::vCameraForward)) * RaisinEng::fCameraSpeed;
		if (_key == GLFW_KEY_R && _action == GLFW_PRESS)
			RaisinEng::vCameraPosition = glm::vec3{ 0.f };
		if (_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);
	});
	InitSoundSystem();
	mCubemap.LoadCubeData();
}

void RaisinEng::Editor_Init()
{
	// INICIALIZAMOS IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	const ImGuiIO& input_output = ImGui::GetIO();
	(void)input_output;
	// INICIALIZAMOS EL ESTULO DE IMGUI
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 1.f;
	// ESTABLECEMOS LA PATAFORMA Y RENDER
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void RaisinEng::_Loop()
{
	sSphere mPrimitive;
	float currentTimeFrame = 0, lastTimeFrame = 0, deltaTime = 0;
	while (!glfwWindowShouldClose(m_window)) // Game loop
	{
		currentTimeFrame += glfwGetTime();
		deltaTime = currentTimeFrame - lastTimeFrame;
		if(deltaTime >= 0.016f)
		{
			mPercentileFPS[mFrameCPerf] = deltaTime;
			++mFrameCPerf;
			_ClearColorBuffer();
			// Process Systems (Ticks)
			//SoundSysTick(deltaTime);
			// Renderables
			mProjectionMatrix = glm::perspective(glm::radians(40.f),
				(800.f / 600.f), 0.3f, 100.f);
			mViewMatrix = glm::lookAt(vCameraPosition, vCameraPosition + vCameraForward,
				vCameraUp);
			//mPrimitive._draw(mModelMatrix, mViewMatrix, mProjectionMatrix, vCameraPosition);
		// TODO: Ordenar los modelos por Material > Shaders
			for (size_t i = 0; i < iCurrentObjs; i++)
			{
				oObjsToDraw[i].Draw();
			}
			mCubemap.Draw(mModelMatrix, mViewMatrix, mProjectionMatrix, vCameraPosition);
	#ifdef _OPENGL
			Editor_Loop();
	#endif
			glfwSwapBuffers(m_window);
			glfwPollEvents();
			lastTimeFrame = currentTimeFrame;
			currentTimeFrame = 0.f;
		}
	}
}

void RaisinEng::Editor_Loop()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// Filedialog texture
	ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
		GLuint tex;

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		return (void*)tex;
	};
	ifd::FileDialog::Instance().DeleteTexture = [](void* tex) {
		GLuint texID = (GLuint)tex;
		glDeleteTextures(1, &texID);
	};
	//-- Filedialog texture
	ImGui::Begin("World");
	{
		if (ImGui::Button("Open a model"))
			ifd::FileDialog::Instance().Open("ModelOpenDialog", "Open a model", "*.gltf, *.glb, *.obj");
		if (ImGui::Button("Load cubemap"))
			ifd::FileDialog::Instance().Open("OpenCubemapDialog", "Open a cumebap texture", "*.png");
		float tempLightPos[3] = {vLightPosition.x, vLightPosition.y, vLightPosition.z};
		ImGui::DragFloat3("Light Position", tempLightPos);
		vLightPosition.x = tempLightPos[0];
		vLightPosition.y = tempLightPos[1];
		vLightPosition.z = tempLightPos[2];
		ImGui::End();
	}
	ImGui::Begin("Visualizer");
	{
		ImGui::SliderFloat("Volume SFX", GetSetVolumeSFX(), 0.f, 1.f, "%.2f");
		if (ImGui::Button("Play SFX"))
			PlaySFX("resources/Sound/NULL.ogg");
		ImGui::SliderFloat("Volume MUSIC", GetSetVolumeMUSIC(), 0.f, 1.f, "%.2f");
		ImGui::PlotLines("Sound Wave", GetSoundWave(), 12);
		ImGui::NewLine();
		// Performance
		ImGui::PlotHistogram("Frame time", mPercentileFPS, 255);
		ImGui::End();
	}
	ImGui::Begin("DEBUG PANEL");
	{
		ImGui::Text("%s", _DebugPanel.c_str());
		ImGui::End();
	}

	ImGui::Begin("INFO PANEL");
	{
		ImGui::Text("%s", _InfoPanel.c_str());
		ImGui::End();
	}

	ImGui::Begin("ERROR PANEL");
	{
		ImGui::Text("%s", _ErrorPanel.c_str());
		ImGui::End();
	}

	if (ifd::FileDialog::Instance().IsDone("ModelOpenDialog")) {
		if (ifd::FileDialog::Instance().HasResult()) {
			std::string res = ifd::FileDialog::Instance().GetResult().u8string();

			oObjsToDraw[iCurrentObjs].AddModel(res);
			++iCurrentObjs;
		}
		ifd::FileDialog::Instance().Close();
	}
	if (ifd::FileDialog::Instance().IsDone("OpenCubemapDialog")) {
		if (ifd::FileDialog::Instance().HasResult()) {
			std::string res = ifd::FileDialog::Instance().GetResult().u8string();
			_InfoPanel += res.c_str();
			_InfoPanel += "\n";
		}
		ifd::FileDialog::Instance().Close();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void RaisinEng::_Destroy()
{
#ifdef _OPENGL
	Editor_Close();
#endif
	glfwTerminate();
	DestroySoundSystem();
}

void RaisinEng::Editor_Close()
{
	ax::NodeEditor::DestroyEditor(m_Context);
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void RaisinEng::RenderObj::Draw()
{
	// Hacemos solo un UseShader, pero pintamos varios modelos.
	for (int i = 0; i < 256; i++)
	{
		mModels[i].Draw(mViewMatrix, mProjectionMatrix, vCameraPosition);
	}
}	 