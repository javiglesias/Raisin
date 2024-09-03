#pragma once
#ifndef C__MESH_H
#define C__MESH_H
#include "glm.hpp"
#include <vector>
#include "Material.h"

struct Vertex;
class Material;

class Mesh 
{
	unsigned int VAO, VBO, EBO;
public:
	Mesh() {}
	void Draw(Material* _Material, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position);
	void setupMesh();// max materials for models
public:
	size_t mNumVertices;
	size_t mNumIndices;
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	unsigned int mMaterialid;
};
#endif
