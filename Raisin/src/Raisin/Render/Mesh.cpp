#include "Mesh.h"
#include "RaiEng.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures)
{
	vertices = _vertices;
	indices  = _indices;
	textures = _textures;
	setupMesh();
}

void Mesh::Draw(Material* _Material, glm::mat4 model, glm::mat4 view, 
	glm::mat4 projection, glm::vec3 camera_position)
{
	
	_DrawElements(model, view, projection, camera_position, _Material,
		VAO, GL_TRIANGLES, indices.size());
}

void Mesh::setupMesh()
{
	VAO = _CreateBuffers(indices, vertices);
}
