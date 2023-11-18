#include "Model.h"
#include "RaiEng.h"
// GLAD goes first
#include "glad/glad.h"
// GLFW goes second
#include "GLFW/glfw3.h" 
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../stb_image/stb_image.h"


void Model::Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position, glm::vec3 _lightPosition,
	glm::vec3 _lightColor)
{
	// Recorremos todas las meshes del modelo y las pintamos.
	if (mScene != nullptr)
	{
		mScene->Draw(view, projection, camera_position, _lightPosition, _lightColor);
		return;
	}

	glm::mat4 model = translate(glm::mat4{ 1.f }, mPosition);
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(mMaterial, model, view, projection, camera_position, _lightPosition, _lightColor);
	}
}

bool Model::loadModel(std::string _path)
{
	const char* _pathChar = _path.c_str();
	const int _pathSize = _path.size();
	/*if (_pathChar[_pathSize - 4] == 'g' &&
		_pathChar[_pathSize - 3] == 'l' &&
		_pathChar[_pathSize - 2] == 't' &&
		_pathChar[_pathSize - 1] == 'f')
	{
		mScene = new cglTFFile(_path);
	} else*/
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cerr << "Error loading the model " << importer.GetErrorString() << '\n';
			return false;
		}
		directory = _path.substr(0, _path.find_last_of('/'));
		directory = directory + '/';
		processNode(scene->mRootNode, scene);
	}
	return true;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int  i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex{};
		try
		{
			vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			if(mesh->mNormals != nullptr)
				vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			if (mesh->mTextureCoords[0])
			{
				vertex.texcoord = glm::vec2(mesh->mTextureCoords[0]->x, mesh->mTextureCoords[0]->y);
			}
			else
			{
				vertex.texcoord = glm::vec2(0.f, 0.f);
			}
		}
		catch (std::exception ex)
		{
			std::cerr << "Error reading Position, normals or texcoords" << ex.what() << '\n';
		}
		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	if (mesh->mMaterialIndex > 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
		std::vector<Texture> specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

	}
	else
	{
		aiMaterial* material{};
		if (strcmp(customTexture, "") != 0)
		{
			std::string sCustomTexture(customTexture);
			sCustomTexture += ".png";
			textures.push_back(LoadCustomTexture(sCustomTexture));

			sCustomTexture = customTexture;
			sCustomTexture = sCustomTexture + "_diffuse.png";
			textures.push_back(LoadCustomTexture(sCustomTexture));

			sCustomTexture = customTexture;
			sCustomTexture = sCustomTexture + "_specular.png";
			textures.push_back(LoadCustomTexture(sCustomTexture));
		}

	}
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType texture_type, std::string name)
{
	std::vector<Texture> textures;
	// Hacer un bucle que compruebe si la textura que se intenta cargar, ya estaba cargada en memoria, para optimizar.
	for (unsigned int i = 0; i < mat->GetTextureCount(texture_type); i++)
	{
		aiString str;
		mat->GetTexture(texture_type, i, &str);
		Texture tex;
		tex.id = TextureFromFile(str.C_Str(), directory);
		tex.type = name;
		tex.path = str.C_Str();
		tex.heigth = heigth;
		tex.width = width;
		tex.nr_channels = nr_channels;
		textures.push_back(tex);
	}
	return textures;
}

Texture Model::LoadCustomTexture(std::string _CustomTexture)
{
	Texture tex;
	tex.id = TextureFromFile(_CustomTexture);
	tex.type = customTexture;
	tex.path = customTexture;
	tex.heigth = heigth;
	tex.width = width;
	tex.nr_channels = nr_channels;
	return tex;
}

unsigned int Model::TextureFromFile(std::string str, std::string directory)
{
	stbi_set_flip_vertically_on_load(false);
	directory.append(str);
	unsigned char* texture_data = stbi_load(directory.c_str(),
		&width, &heigth, &nr_channels, 0);
	unsigned int texture = -1;
	if(texture_data)
	{
		texture = _CreateTexture(texture_data, width, heigth);
		stbi_image_free(texture_data);
	}
	return texture;
}

unsigned int Model::TextureFromFile(std::string str)
{
	stbi_set_flip_vertically_on_load(false);
	unsigned char* texture_data = stbi_load(str.c_str(),
		&width, &heigth, &nr_channels, 0);
	unsigned int texture = -1;
	if (texture_data)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture_data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(texture_data);
	}
	return texture;
}
