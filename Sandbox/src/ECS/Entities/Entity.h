#ifndef C__ENTITY_H
#define C__ENTITY_H

#include "../../Render/Shader.h"
#include "../../common.h"


#define SHAPE 12
struct Color
{
	float r = 0.f, g = 0.f, b = 0.f, a = 1.f;
	Color(){}
	Color(float _r, float _g, float _b, float _a = 1.f) 
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
};
typedef struct Shape_Size
{
	float w;
	float h;
	Shape_Size()
	{
		w = -1;
		h = -1;
	}
	Shape_Size(float _w, float _h)
	{
		w = _w;
		h = _h;
	}
} Shape_Size;

class Entity
{
public:
	enum STATE
	{
		INVALID,
		READY
	};
	bool isHero = false;
	Entity(){}
	virtual ~Entity()
	{
		printf("~ENTITY\n");
	}
	void virtual Draw() { }
	void virtual DebugDraw() {}
	void virtual Tick(float _timediff) {}
	void virtual DrawEffectArea(){}
	void virtual LoadTexture(){}

	bool m_to_delete = false;
	bool m_show = true;
	STATE m_state;
	float m_shape[SHAPE] = {
		//	x,y,z
		0.0f, 0.0f, 0.0f,
		-0.02f, -0.05f, 0.0f,
		0.02f, -0.05f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float m_collisionShape[24] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.1f, 0.0f,

		0.0f, 0.1f, 0.0f,
		0.1f, 0.1f, 0.0f,

		0.1f, 0.1f, 0.0f,
		0.1f, 0.0f, 0.0f,

		0.1f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};
	glm::vec3 m_transform {0.f};

private:
	float m_width = 0.1f;
	float m_height = 0.1f;
	const char* m_name = "Defualt Entity";
};

#endif
