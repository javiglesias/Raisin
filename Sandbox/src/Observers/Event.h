#pragma once
enum Type
{
	SAVE_ENTITY,
	DEATH,
	REVIVE
};
class Event
{
public:
	Type m_type;
};

