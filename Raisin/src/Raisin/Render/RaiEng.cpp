#include "RaiEng.h"

#include "../imgui/ImFileDialog.h"

// Dedicated GPU for laptops with 2 https://stackoverflow.com/questions/16823372/forcing-machine-to-use-dedicated-graphics-card
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

void RaisinEng::_Init(int _Width, int _Height, const char* _AppName)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE,
		GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(_Width, _Height, _AppName,
#ifdef _GAME
		glfwGetPrimaryMonitor(),
#else
		nullptr,
#endif
		nullptr);
	if (m_window == nullptr)
	{
		glfwTerminate();
		exit(-1);
	}
	//	GLAD
	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		exit(-1);
	}
	glViewport(0, 0, _Width, _Height);

	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(m_window, mouse_press_callback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	Editor_Init();
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
	ImGui_ImplOpenGL3_Init("#version 430");
}

void RaisinEng::_Loop()
{
	while (!glfwWindowShouldClose(m_window)) // Game loop
	{
		process_input(m_window);
		glClearColor(0, 1.f, 0, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		Editor_Loop();
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void RaisinEng::Editor_Loop()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
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
	ImGui::Begin("World");
	{
		if (ImGui::Button("Open a texture"))
			ifd::FileDialog::Instance().Open("TextureOpenDialog", "Open a texture", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*");
		ImGui::End();
	}

	if (ifd::FileDialog::Instance().IsDone("TextureOpenDialog")) {
		if (ifd::FileDialog::Instance().HasResult()) {
			std::string res = ifd::FileDialog::Instance().GetResult().u8string();
			printf("OPEN[%s]\n", res.c_str());
		}
		ifd::FileDialog::Instance().Close();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}