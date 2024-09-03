#include "Mesh.h"
#include "RaiEng.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

void Mesh::Draw(Material* _Material, glm::mat4 model, glm::mat4 view, 
	glm::mat4 projection, glm::vec3 camera_position)
{
	_DrawElements(model, view, projection, camera_position, _Material,
		VAO, GL_TRIANGLES, mNumIndices);
	if(mNumIndices < 0)
		_DrawArrays(model, view, projection, camera_position, _Material,
			VAO, GL_TRIANGLES, 0, 0);
}

void Mesh::setupMesh()
{
	mNumVertices =  mVertices.size();
	mNumIndices = mIndices.size();
	VAO = _CreateBuffers( mNumIndices , &mIndices[0], mNumVertices, &mVertices[0], &VAO, &VBO, &EBO);

}
