#include "Monster.h"

Monster::Monster()
{
	Entity::m_name = "dummyMonster";
}

Monster::~Monster()
{
	//delete m_brain;
	//delete m_ears;
	/*delete m_shader;
	delete m_tex_data;*/
}

Monster::Monster(glm::vec3 _init_position, uint8_t _health)
{
	m_shader = new Shader("code\\shaders\\basic.vert", "code\\shaders\\basic.frag", "MOSTRO");
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO_Circle);
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO_Circle);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_shape), m_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);
	model_id = glGetUniformLocation(m_shader->id, "model");
	base_color_id = glGetUniformLocation(m_shader->id, "base_color");
	alive_id = glGetUniformLocation(m_shader->id, "is_alive");

	g_physics_components[g_total_physics_component] = new PhysicsComponent(_init_position, 0.04f, 0.08f, false);
	m_transform = g_physics_components[g_total_physics_component];
	++g_total_physics_component;

	g_ai_components[g_total_ai_component] = new AIComponent();
	m_brain = g_ai_components[g_total_ai_component];
	++g_total_ai_component;
	m_Health = _health;
}

void Monster::Draw()
{
	glBindVertexArray(VAO);
	if (m_transform->m_isColliding)
	{
		--m_Health;
		glUniform4f(base_color_id, 1, 0, 0, 1);
		//DrawEffectArea();
	}
	else
		glUniform4f(base_color_id, 0, 1, 0, 1);
	glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(m_transform->m_model));
	if (m_Health > 0 && !m_transform->m_hasEscape)
	{
		m_shader->use();
		glDrawArrays(GL_TRIANGLES, 0, 4);
		m_shader->unuse();
	}
	else
	{
		if (!m_transform->m_hasEscape)
			++g_current_level_enemies_dead;
		else
			++g_current_level_enemies_escaped;
		ResetMonster();
	}
}

void Monster::DrawEffectArea()
{
	glBindVertexArray(VAO_Circle);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Circle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_effect_area), m_effect_area, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);
	glUniform4f(base_color_id, 1, 1, 1, 1);
	glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(m_transform->m_model));
	m_shader->use();
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	m_shader->unuse();
}

void Monster::ResetMonster()
{
	m_Health = 1;
	float x_pos = GetRandomInt() / 255.f;
	float y_pos = GetRandomInt() / 255.f;
	if(GetRandomBoolean() && GetRandomBoolean())
		m_transform->m_position = glm::vec3(x_pos, y_pos, 0.f);
	else if (GetRandomBoolean() && !GetRandomBoolean())
		m_transform->m_position = glm::vec3(x_pos, -y_pos, 0.f);
	else if (!GetRandomBoolean() && GetRandomBoolean())
		m_transform->m_position = glm::vec3(-x_pos, y_pos, 0.f);
	else if (!GetRandomBoolean() && !GetRandomBoolean())
		m_transform->m_position = glm::vec3(-x_pos, -y_pos, 0.f);
	m_transform->m_hasEscape = false;
}
