#ifndef C__MODEL_H
#define C__MODEL_H

#include <vector>
#include <string>
#include <assimp/scene.h>

#include "Mesh.h"
#include "Shader.h"

class Model
{
public:
	Model(){}
	Model(std::string path)
	{
		loadModel(path);
	}
	void Draw(Shader* _Shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection, 
		glm::vec3 camera_position, glm::vec3 _lightPosition);
private:
	char customTexture[128] = "resources/textures/WoddenBox"; // "customTexture"_specular.png
	std::vector<Mesh> meshes;
	std::string directory;
	int width, heigth, nr_channels;
	bool loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial*, aiTextureType, std::string);
	Texture LoadCustomTexture(std::string _CustomTexture);
	unsigned int TextureFromFile(std::string, std::string);
	unsigned int TextureFromFile(std::string _texturePath);
};
#endif // !C__MODEL_H