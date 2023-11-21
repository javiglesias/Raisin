#include "Room.h"


void Room::Init(glm::vec2 _init_position, float _width, float _height)
{
	int w = 0, h = 0, nChannels = 0;
	m_shape_size = Shape_Size(_width, _height);
	unsigned char* data = nullptr;
#if 0
	strcpy(m_texturePath, "resources/art/dirt.jpg");
	data = stbi_load(m_texturePath, &w, &h, &nChannels, 0);
#endif


	// witdth
	m_shape[6] = _width;
	m_shape[9] = _width;
	m_shape[15] = _width;
	//height
	m_shape[4] = _height;
	m_shape[13] = _height;
	m_shape[16] = _height;
	m_shader = new Shader("resources/shaders/basic.vert", "resources/shaders/basic.frag", "WATER");

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_shape), m_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);

	m_transform = glm::vec3(_init_position.x * _width,
		_init_position.y * _height, 0.f);
	m_model = translate(m_model, m_transform);
#if 0
	if (data)
	{
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(texture_id);
		stbi_image_free(data);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 2 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
#endif
	m_state = READY;
}

Room::~Room()
{
	printf("~Room\n");
}


void Room::Draw()
{
	if (m_state == INVALID) return;
	model_id      = glGetUniformLocation(m_shader->id, "model");
	base_color_id = glGetUniformLocation(m_shader->id, "color");

	m_model = glm::mat4(1.f);
	m_transform += g_scrolling * g_DefaultMovFactor;
	m_model = translate(m_model, m_transform);

	if ( m_transform.x > .99f || m_transform.x < -.99f
		|| m_transform.y > .99f || m_transform.y < -.99f)
		return;

	if (model_id < 0) return;

	glBindVertexArray(VAO);
	glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(m_model));
	glUniform4f(base_color_id, 1, 0, 0, 1);
	unsigned int iTime_uniform = glGetUniformLocation(m_shader->id, "iTime");
	if (iTime_uniform >= 0)
		glUniform1f(iTime_uniform, iTime);
	m_shader->use();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_shader->unuse();
}

void Room::Tick(float _timediff)
{
	Entity::Tick(_timediff);
	iTime += _timediff;
}

void Room::SetShaderColor(Color _color)
{
	glUniform4f(base_color_id, _color.r, _color.g, _color.b, _color.a);
	m_shader_base_color = _color;
}
