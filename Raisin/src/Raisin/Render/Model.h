#ifndef C__MODEL_H
#define C__MODEL_H
#include "Material.h"
#include <string>
#include "glm.hpp"

class cgltf_data;
class Mesh;
class Texture;
class Model
{
public:
	Model(const char* _filepath, const char*  _modelName);
	void Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position);
private:
	unsigned int TextureFromFile(std::string, std::string);
	unsigned int TextureFromFile(std::string _texturePath);
public:
	char mName[64];
	bool mEditing = false;
	int width, heigth, nr_channels;
	Mesh* mMeshes[256];
	Material mMaterials[256];
	int iCurrentMeshes = 0;
	glm::vec3 mPosition {0.f};
	glm::vec3 mScale {1.f};
	glm::vec3 mColor{ 0.f };
};
#endif // !C__MODEL_H