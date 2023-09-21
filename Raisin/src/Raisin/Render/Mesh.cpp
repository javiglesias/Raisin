#include "Mesh.h"
#include "RaiEng.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures)
{
	vertices = _vertices;
	indices  = _indices;
	textures = _textures;
	setupMesh();
}

void Mesh::Draw(Shader* _Shader, glm::mat4 model, glm::mat4 view, 
	glm::mat4 projection, glm::vec3 camera_position, glm::vec3 _lightPosition)
{
	glPolygonMode(GL_FRONT, GL_FILL);
	_DrawElements(model, view, projection, camera_position, _lightPosition, _Shader->id, VAO, GL_TRIANGLES, indices.size());
}

void Mesh::setupMesh()
{
	VAO = _CreateBuffers(indices, vertices);
}
