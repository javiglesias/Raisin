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
//API functions
void RaisinEng::_process_input(GLFWwindow* _window)
{
	glfwSetWindowShouldClose(_window, glfwGetKey(_window, GLFW_KEY_ESCAPE));
	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		if (glfwGetKey(_window, i))
		{
			_process_input(i);
		}
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
	_add_callback_input( eINPUTKEY(GLFW_KEY_W), []()
	{
			RaisinEng::vCameraPosition += RaisinEng::fCameraSpeed * RaisinEng::vCameraForward;
	});
	_add_callback_input(eINPUTKEY(GLFW_KEY_A), []()
	{
			RaisinEng::vCameraPosition += glm::normalize(glm::cross(
				RaisinEng::vCameraUp, RaisinEng::vCameraForward)) * RaisinEng::fCameraSpeed;
	});
	_add_callback_input(eINPUTKEY(GLFW_KEY_S), []()
	{
			RaisinEng::vCameraPosition -= RaisinEng::fCameraSpeed * RaisinEng::vCameraForward;
	});
	_add_callback_input(eINPUTKEY(GLFW_KEY_D),[]()
	{
			RaisinEng::vCameraPosition -= glm::normalize(glm::cross(
				RaisinEng::vCameraUp, RaisinEng::vCameraForward)) * RaisinEng::fCameraSpeed;
	});
	_add_callback_input(eINPUTKEY(GLFW_KEY_R),[]()
	{
			RaisinEng::vCameraPosition = glm::vec3{0.f};
	});

	//Models && "Materials"
	oModelMaterial = new Material("basic_shader", "basic_shader", false, glm::vec3(1.f));
	oLightMaterial = new Material("basic_shader", "basic_shader", false, glm::vec3(0.f));

	oLight = new Model("resources/models/BasicShapes/LightBulb.obj", oLightMaterial, oMeshes);

	/*oObjsToDraw[iCurrentObjs].AddModel("resources/models/backpack/backpack.obj", oModelMaterial);	
	++iCurrentObjs;*/
	InitSoundSystem();
	mCubemap.LoadTexData();
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
		lastTimeFrame = currentTimeFrame;
		if(deltaTime >= 0.016f)
		{
			currentTimeFrame = 0.f;
			_process_input(m_window);
			_ClearColorBuffer();
			// Process Systems (Ticks)
			SoundSysTick(deltaTime);
			// Renderables
			mProjectionMatrix = glm::perspective(glm::radians(40.f),
				(800.f / 600.f), 0.3f, 100.f);
			mViewMatrix = glm::lookAt(vCameraPosition, vCameraPosition + vCameraForward,
				vCameraUp);
			//mPrimitive._draw(mModelMatrix, mViewMatrix, mProjectionMatrix, vCameraPosition);
			mCubemap.Draw(mModelMatrix, mViewMatrix, mProjectionMatrix, vCameraPosition);
		// TODO: Ordenar los modelos por Material > Shaders
			for (size_t i = 0; i < iCurrentObjs; i++)
			{
				oObjsToDraw[i].Draw();
			}
			oLight->Draw(mViewMatrix, mProjectionMatrix, vCameraPosition, vLightPosition, oLight->GetMaterial()->mLightColor);
	#ifdef _OPENGL
			Editor_Loop();
	#endif
			glfwSwapBuffers(m_window);
			glfwPollEvents();
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
		oLight->mPosition = vLightPosition;

		float tempLightColor[3] = { oLight->GetMaterial()->mLightColor.x,
			oLight->GetMaterial()->mLightColor.y, oLight->GetMaterial()->mLightColor.z };
		ImGui::ColorEdit3("Light Color", tempLightColor);
		oLight->GetMaterial()->mLightColor.x = tempLightColor[0];
		oLight->GetMaterial()->mLightColor.y = tempLightColor[1];
		oLight->GetMaterial()->mLightColor.z = tempLightColor[2];
		ImGui::End();
	}
	ImGui::Begin("Sound Visualizer");
	{
		ImGui::SliderFloat("Volume SFX", GetSetVolumeSFX(), 0.f, 1.f, "%.2f");
		ImGui::NewLine();
		if (ImGui::Button("Play SFX"))
			PlaySFX("resources/Sound/NULL.ogg");
		ImGui::NewLine();
		ImGui::SliderFloat("Volume MUSIC", GetSetVolumeMUSIC(), 0.f, 1.f, "%.2f");
		ImGui::NewLine();
		ImGui::PlotLines("Sound Wave", GetSoundWave(), 12);
		ImGui::NewLine();
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

			oObjsToDraw[iCurrentObjs].AddModel(res, oModelMaterial);
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
		mModels[i].Draw(mViewMatrix, mProjectionMatrix, vCameraPosition, vLightPosition, oLight->GetMaterial()->mLightColor);
	}
}	 