#include "Model.h"
#include "RaiEng.h" 
#include "Mesh.h"
//#include "Scene.h"
#include <iostream>
#include "../cgltf/cgltf.h"

void Model::Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position)
{
	glm::mat4 model = glm::mat4{ 1.f }; 
	model = glm::translate(model, mPosition);
	model = glm::rotate<float>(model,9.f, glm::vec3(1, 0,0 ));
	model = glm::scale(model, mScale);

	for (unsigned int i = 0; i < iCurrentMeshes; i++)
		mMeshes[i]->Draw(&mMaterials[mMeshes[i]->mMaterialid],model, view, projection, camera_position);
}

Model::Model(const char* _filepath, const char*  _modelName)
{
	cgltf_options options {};
	char filename[128];
	sprintf(filename, "%s%s", _filepath, _modelName);
	cgltf_data* modelData = NULL;
	cgltf_parse_file(&options, filename, &modelData);
	if(modelData == NULL) exit(-999);
	if(cgltf_load_buffers(&options, modelData, _filepath) != cgltf_result_success)
		__debugbreak();
	strcpy(mName, _modelName);
	if(cgltf_validate(modelData) == cgltf_result_success)
	{
		printf("\nModel file validated (nodes %ld)", modelData->nodes_count);
		//  MATERIALS
		size_t materialID = 0;
		if(modelData->materials_count > 0)
		{
			for (size_t i = 0 ; i < modelData->materials_count; i++) 
			{
				auto material = modelData->materials[i];
				materialID = i;
				fprintf(stderr, "Material %llu: %s\n", materialID, material.name);
				//mMaterials[materialID].m_TextureSpecular = new Texture();
				//mMaterials[materialID].m_TextureDiffuse = new  Texture();
				if(material.normal_texture.texture != nullptr)
				{
					std::string pathTexture = std::string(material.normal_texture.texture->image->uri);
					//mMaterials[materialID].m_TextureDiffuse->m_Path =  _filepath + pathTexture;
				}
				if(material.specular.specular_texture.texture != nullptr)
				{
					std::string pathTexture = std::string(material.specular.specular_texture.texture->image->uri);
					//mMaterials[materialID].m_TextureSpecular->m_Path = _filepath + pathTexture;
				}
				if(material.pbr_metallic_roughness.base_color_texture.texture != nullptr)
				{
					std::string pathTexture = std::string(material.pbr_metallic_roughness.base_color_texture.texture->image->uri);
					//material.pbr_metallic_roughness.metallic_roughness_texturematerial.
					fprintf(stderr, "%s\n",  (_filepath + pathTexture).c_str());
					char path[128];
					sprintf(path, "%s\n",  (_filepath + pathTexture).c_str());
					mMaterials[materialID].mTextureAmbient = new Texture(path);
					//mMaterials[materialID].mTextureAmbient->mPath = _filepath + pathTexture;
					
				}
				mMaterials[materialID].DefaultInit();
				mMaterials[materialID].mState = Material::INITIALIZED;
				/*mMaterials[materialID].m_TextureEmissive = new render::Texture();
				mMaterials[materialID].m_TextureMetallicRoughness = new render::Texture();
				mMaterials[materialID].m_TextureOcclusion = new render::Texture();
				mMaterials[materialID].m_TextureNormal = new render::Texture();
				mMaterials[materialID].m_TextureShadowMap = new render::Texture();*/
			}
		}
		else
		{
			//mMaterials[materialID].m_TextureSpecular = new Texture();
			//mMaterials[materialID].m_TextureDiffuse =	new Texture();
			mMaterials[materialID].mTextureAmbient = new Texture();
		}

		for (cgltf_size n = 0; n < modelData->nodes_count; ++n)
		{
			auto mesh = modelData->nodes[n].mesh;
			auto camera = modelData->nodes[n].camera;
			if(mesh)
			{
				Mesh* tempMesh = new Mesh();
				printf("\n\tMesh %zd %s (%ld)\n", n, mesh->name, mesh->primitives_count);
				for(cgltf_size p = 0; p < mesh->primitives_count; p++)
				{
					printf("\tPrimitive %zd\n", p);
					for(cgltf_size a = 0; a < mesh->primitives[p].attributes_count; a++)
					{
						tempMesh->mMaterialid = cgltf_material_index(modelData, mesh->primitives[p].material);
						auto accessor = mesh->primitives[p].attributes[a].data;
						Vertex* tempVertex = new Vertex();
						if(cgltf_attribute_type_position == mesh->primitives[p].attributes[a].type)
						{
							if(accessor->type == cgltf_type_vec3)
							{
								fprintf(stderr, "vertex count %llu\n", accessor->count);
								auto vertices = new float[3*accessor->count];
								int n = 0; 
								float *buffer = (float *)accessor->buffer_view->buffer->data + accessor->buffer_view->offset/sizeof(float) + accessor->offset/sizeof(float); 
								for (unsigned int k = 0; k < accessor->count; k++) 
								{
									float tempVrtx[3];
									for (int l = 0; l < 3; l++) 
									{
										vertices[3*k + l] = buffer[n + l];
										tempVrtx[l] = buffer[n + l];
									}
									tempVertex->mPosition= glm::vec3(tempVrtx[0], tempVrtx[1], tempVrtx[2]);
									if(tempMesh->mVertices.size() <= k)
									{
										tempVertex = new Vertex();
										tempMesh->mVertices.push_back(*tempVertex);
									}
									else
										tempMesh->mVertices[k].mPosition = tempVertex->mPosition;
									n += (int)(accessor->stride/sizeof(float));
								}
							}
						}
						else if(cgltf_attribute_type_normal == mesh->primitives[p].attributes[a].type)
						{
							if(accessor->type == cgltf_type_vec3)
							{
								fprintf(stderr, "normal count %llu\n", accessor->count);
								float* values = new float[3*accessor->count];
								int n = 0; 
								float *buffer = (float *)accessor->buffer_view->buffer->data + accessor->buffer_view->offset/sizeof(float) + accessor->offset/sizeof(float); 
								for (unsigned int k = 0; k < accessor->count; k++) 
								{
									float tempNormal[3];
									for (int l = 0; l < 3; l++) 
									{
										values[3*k + l] = buffer[n + l];
										tempNormal[l] = buffer[n + l];
									}
									tempVertex->mNormal = glm::vec3(tempNormal[0], tempNormal[1], tempNormal[2]);
									if(tempMesh->mVertices.size() <= k)
									{
										tempVertex = new Vertex();
										tempMesh->mVertices.push_back(*tempVertex);
									}
									else
										tempMesh->mVertices[k].mNormal = tempVertex->mNormal;
									n += (int)(accessor->stride/sizeof(float));
								}
							}
						}
						else if(cgltf_attribute_type_tangent == mesh->primitives[p].attributes[a].type)
						{

						}
						else if(cgltf_attribute_type_texcoord == mesh->primitives[p].attributes[a].type)
						{
							if(accessor->type == cgltf_type_vec2)
							{
								fprintf(stderr, "Texcoord count %llu\n", accessor->count);
								float* values = new float[2*accessor->count];
								int n = 0; 
								float *buffer = (float *)accessor->buffer_view->buffer->data + accessor->buffer_view->offset/sizeof(float) + accessor->offset/sizeof(float); 
								for (unsigned int k = 0; k < accessor->count; k++) 
								{
									float tempTexCoord[2];
									for (int l = 0; l < 2; l++) 
									{
										values[2*k + l] = buffer[n + l];
										tempTexCoord[l] = buffer[n + l];
									}
									tempVertex->mTexcoord = glm::vec2(tempTexCoord[0], tempTexCoord[1]);
									if(tempMesh->mVertices.size() <= k)
									{
										tempVertex = new Vertex();
										tempMesh->mVertices.push_back(*tempVertex);
									}
									else
										tempMesh->mVertices[k].mTexcoord = tempVertex->mTexcoord;
									n += (int)(accessor->stride/sizeof(float));
								}
							}
						}
						else 
						{
							fprintf(stderr,  "Deshecho: %s\n", mesh->primitives[p].attributes[a].name);
						}
					}
					if(mesh->primitives[p].indices != NULL)
					{
						int n = 0; 
						auto accessor = mesh->primitives[p].indices;
						unsigned short index;
						unsigned short *buffer = (unsigned short *)accessor->buffer_view->buffer->data + accessor->buffer_view->offset/sizeof(unsigned short) + accessor->offset/sizeof(unsigned short); 
						for (unsigned int k = 0; k < accessor->count; k++) 
						{
							index = buffer[n];
							// fprintf(stderr, "Index %d: ",index );
							tempMesh->mIndices.push_back(index);
							n += (int)(accessor->stride/sizeof(unsigned short));
						}
					}
				}
				tempMesh->setupMesh();
				mMeshes[iCurrentMeshes] = tempMesh;
				++iCurrentMeshes;
			}
		}
	}
}