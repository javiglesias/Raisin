#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "RenderAPI.h"	

struct sCubemap
{
	unsigned char*	mTextureData;
	unsigned int	mTextureId = -1;
	unsigned int	mVAO = -1;
	unsigned int	mShaderID = -1;
	unsigned int	mTexture = -1;
	std::vector<Vertex*> mVertices;
	std::vector<unsigned int> mIndices;
	Material		mMaterial;
	int mWidth		= 0;
	int mHeigth		= 0;
	int mNChannels	= 0;
	const char mTexPath[128] = "resources/textures/cubemaps/office.png";
	const char mCubePath[128] = "resources/models/backpack/backpack.obj";
	float mCubemapVertices[33];

	sCubemap(){}
	void LoadCubeData()
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(mCubePath, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cerr << "Error loading the model " << importer.GetErrorString() << '\n';
			exit(-1);
		}
		processNode(scene->mRootNode, scene);
		LoadTexData();
		mTexture = _CreateTextureFromFile(mTexPath);
		//mMaterial.Init("cubemap", "cubemap");
		//mMaterial.mTextureId = mTexture;
	}

	void LoadTexData()
	{
		mTextureId = _CreateTextureFromFile(mTexPath);
		LoadVAOData();
		/*mTextureData = stbi_load(mTexPath,
			&mWidth, &mHeigth, &mNChannels, 0);
		if(mTextureData)
		{
			stbi_image_free(mTextureData);
		}*/
	}

	void LoadVAOData()
	{
		mVAO = _CreateBuffers(mIndices, mVertices);
	}

	void Draw(glm::mat4 _ModelMatrix, glm::mat4 _ViewMatrix, glm::mat4 _ProjectionMatrix, glm::vec3 _CameraPosition)
	{
		glm::mat4 model{1.f};
		glm::scale(model, glm::vec3{50.f});
		//mMaterial.mTextureId = mTextureId;
		_DrawElements(_ModelMatrix, _ViewMatrix, _ProjectionMatrix, _CameraPosition, &mMaterial, mVAO, GL_TRIANGLES,
			mIndices.size());
	}

	void processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex* vertex = new Vertex();
				try
				{
					vertex->mPosition = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
					if (mesh->mNormals != nullptr)
						vertex->mNormal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
					if (mesh->mTextureCoords[0])
					{
						vertex->mTexcoord = glm::vec2(mesh->mTextureCoords[0]->x, mesh->mTextureCoords[0]->y);
					}
					else
					{
						vertex->mTexcoord = glm::vec2(0.f, 0.f);
					}
				}
				catch (std::exception ex)
				{
					std::cerr << "Error reading Position, normals or texcoords" << ex.what() << '\n';
				}
				mVertices.push_back(vertex);
			}
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					mIndices.push_back(face.mIndices[j]);
				}
			}
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
};