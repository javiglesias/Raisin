#ifndef C__MODEL_H
#define C__MODEL_H

#include <vector>
#include <string>
#include <assimp/scene.h> // assimp
#include <assimp/mesh.h> // assimp
#include <assimp/material.h> //assimp

#include "Mesh.h"
#include "Material.h"
#include "Scene.h"

class Model
{
public:
	Model(){}
	Model(std::string path, std::vector<Mesh>& oMeshes)
	{
		mMaterial = _CreateMaterial("resources\\models\\BasicShapes\\backpack.mtl");;
		meshes = oMeshes;
		loadModel(path);
	}
	Material* GetMaterial() const { return mMaterial; }
	void Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position);
private:
	unsigned int materialLastIndex = -1;
	char customTexture[128] = ""; // "customTexture"_specular.png
	int width, heigth, nr_channels;
	std::vector<Mesh> meshes;
	std::string directory;
	Material* mMaterial;
	cglTFFile* mScene = nullptr;
public:
	glm::vec3 mPosition {0.f};
	glm::vec3 mColor{ 0.f };
private:
	bool loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial*, aiTextureType, std::string);
	Texture LoadCustomTexture(std::string _CustomTexture);
	unsigned int TextureFromFile(std::string, std::string);
	unsigned int TextureFromFile(std::string _texturePath);
};
#endif // !C__MODEL_H