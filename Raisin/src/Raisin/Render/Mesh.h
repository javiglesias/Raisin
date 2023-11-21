#pragma once
#ifndef C__MESH_H
#define C__MESH_H
#include "glm.hpp"
#include <vector>
#include "Material.h"

struct Vertex;

class Mesh 
{
	unsigned int VAO, VBO, EBO;

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Material* _Material, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position);

private:
// RENDER DATA.
	void setupMesh();

public:
	std::vector<Texture> textures;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};
#endif
