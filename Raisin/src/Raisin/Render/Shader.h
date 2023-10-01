#ifndef C__SHADER_H
#define C__SHADER_H
#include <string>
#include "glm.hpp"
class Shader
{
public:
	unsigned int id = -1;
	Shader(char* _vertex_path, char* _fragment_path);
	Shader(){}
	~Shader()
	{
		printf("~SHADER\n");
	}
	void use();
	void setColor(glm::vec3 _color) { mColor = _color; }
	void setBool(const std::string &name, bool value) const;
	void setFloat(const std::string &name, float value) const;
	void setInt(const std::string& name, int value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
private:
	const char* name="DEFAULT SHADER";
	glm::vec3 mColor = glm::vec3(0);
	std::string mVertShaderCode;
	std::string mFragShaderCode;
private:
	void CompileShader(char* _path);
};
#endif
