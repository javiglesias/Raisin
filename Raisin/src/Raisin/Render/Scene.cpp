#include "Scene.h"
#include <fstream>
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "../DebugUtils/Debug.h"
#define CGLTF_IMPLEMENTATION
#include "../../cgltf/cgltf.h"


cglTFFile::cglTFFile(std::string _path)
{
#if 0
	const char* _pathChar = _path.c_str();
	const int _pathSize = _path.size();
	if (_pathChar[_pathSize - 4] == 'g' &&
		_pathChar[_pathSize - 3] == 'l' &&
		_pathChar[_pathSize - 2] == 't' &&
		_pathChar[_pathSize - 1] == 'f')
	{
		nlohmann::json _gltfFile;
		std::ifstream _file(_path);
		_gltfFile = nlohmann::json::parse(_file);
		if (_gltfFile.is_object())
		{
			auto itms = _gltfFile.items();
			for (auto it : itms)
			{
				if ( strcmp(it.key().c_str(), "cameras") == 0)
				{
					//printf("Cameras\n");
				}
				if ( strcmp(it.key().c_str(), "nodes") == 0)
				{
					//printf("Nodes\n");
					auto nodes = it.value().items();
					for (auto node : nodes)
					{
						sNode nodeTemp;
						if(!node.value()["name"].is_null())
							nodeTemp.mName = node.value()["name"].get<std::string>();
						if (!node.value()["mesh"].is_null())
							nodeTemp.mMesh = node.value()["mesh"].get<unsigned int>();
						//printf("node: %s\n", nodeTemp.mName.c_str());
						// Translation
						auto translation = node.value()["translation"].items();
						//printf("\ttranslation");
						for (auto p : translation)
						{
							nodeTemp.mTranslation.push_back(p.value().get<float>());
							//printf(" %f", nodeTemp.mTranslation);
						}
						//printf("\n");
						// Rotation
						auto rotation = node.value()["rotation"].items();
						//printf("\trotation");
						for (auto p : translation)
						{
							nodeTemp.mRotation.push_back(p.value().get<float>());
							//printf(" %f", t);
						}
						//printf("\n");
						// Scale
						auto scale = node.value()["scale"].items();
						//printf("\tscale");
						for (auto p : translation)
						{
							nodeTemp.mScale.push_back(p.value().get<float>());
							//printf(" %f", t);
						}
						//printf("\n");
						mNodes.push_back(nodeTemp);
					}
				}
				if ( strcmp(it.key().c_str(), "scenes") == 0)
				{
					//printf("Scenes\n");
					auto scenes = it.value().items();
					for (auto scene : scenes)
					{
						sScene _scene;
						auto nodes = scene.value()["nodes"].items();
						for (auto node : nodes)
						{
							_scene.mNodes.push_back(node.value().get<unsigned int>());
						}
						mScenes.push_back(_scene);
					}
				}
				if ( strcmp(it.key().c_str(), "scene") == 0)
				{
					//printf("Scene\n");
				}
				if ( strcmp(it.key().c_str(), "meshes") == 0)
				{
					//printf("Meshes\n");
					auto meshes = it.value().items();
					for (auto mesh : meshes)
					{
						sMesh tempMesh;
						//name
						tempMesh.mName = mesh.value()["name"].get<std::string>();
						//primitives
						auto primitives = mesh.value()["primitives"].items();
						for (auto primitive : primitives)
						{
							sMeshPrimitive tempPrimitive;
							//indices
							tempPrimitive.mIndices = primitive.value()["indices"].get<unsigned int>();
							//materials
							tempPrimitive.mMaterial = primitive.value()["material"].get<unsigned int>();
							//attributes
							tempPrimitive.mAttributes.mPosition  = primitive.value()["attributes"]["POSITION"].get<unsigned int>();
							tempPrimitive.mAttributes.mNormal	 = primitive.value()["attributes"]["NORMAL"].get<unsigned int>();
							if(!primitive.value()["attributes"]["TANGENT"].is_null())
								tempPrimitive.mAttributes.mTangent	 = primitive.value()["attributes"]["TANGENT"].get<unsigned int>();
							if (!primitive.value()["attributes"]["TEXCOORD_0"].is_null())
								tempPrimitive.mAttributes.mTexCoord0 = primitive.value()["attributes"]["TEXCOORD_0"].get<unsigned int>();
							if(!primitive.value()["attributes"]["TEXCOORD_1"].is_null())
								tempPrimitive.mAttributes.mTexCoord1 = primitive.value()["attributes"]["TEXCOORD_1"].get<unsigned int>();
							tempMesh.mPrimitives.push_back(tempPrimitive);
						}
						mMeshes.push_back(tempMesh);
					}
				}
				if ( strcmp(it.key().c_str(), "materials") == 0)
				{
					//printf("Materials\n");
				}
				if ( strcmp(it.key().c_str(), "textures") == 0)
				{
					//printf("Textures\n");
				}
				if ( strcmp(it.key().c_str(), "accessors") == 0)
				{
					//printf("Accessors\n");
					auto accessors = it.value().items();
					for(auto accessor : accessors)
					{
						sAccessor tempAccessor;
						// name
						if(!accessor.value()["name"].is_null())
							tempAccessor.mName = accessor.value()["name"].get<std::string>();
						// buffervire
						tempAccessor.mBufferView = accessor.value()["bufferView"].get<unsigned int>();
						// componentType
						tempAccessor.mComponentType = accessor.value()["componentType"].get<unsigned int>();
						// count
						tempAccessor.mCount = accessor.value()["count"].get<unsigned int>();
						// type
						tempAccessor.mType = accessor.value()["type"].get<std::string>();
						// byteOffset
						if(!accessor.value()["byteOffset"].is_null())
							tempAccessor.mByteOffset = accessor.value()["byteOffset"].get<unsigned int>();
						// Max
						//tempAccessor.mType = accessor.value()["name"].get<std::string>();
						// Min
						//tempAccessor.mType = accessor.value()["name"].get<std::string>();
						mAccessors.push_back(tempAccessor);
					}
				}
				if ( strcmp(it.key().c_str(), "bufferViews") == 0)
				{
					printf("bufferViews\n");
					auto buffViews = it.value().items();
					for (auto buffView : buffViews)
					{
						sBufferView bufferview;
						// "buffer"
						if (!buffView.value()["buffer"].is_null())
							bufferview.mBuffer = buffView.value()["buffer"].get<unsigned int>();
						// "byteLength"
						if (!buffView.value()["byteLength"].is_null())
							bufferview.mByteLength = buffView.value()["byteLength"].get<unsigned int>();
						// "byteOffset"
						if (!buffView.value()["byteOffset"].is_null())
							bufferview.mByteOffset = buffView.value()["byteOffset"].get<unsigned int>();
						// byteStride
						if (!buffView.value()["byteStride"].is_null())
							bufferview.mByteStride = buffView.value()["byteStride"].get<unsigned int>();
						// "target"
						if (!buffView.value()["name"].is_null())
							bufferview.mName = buffView.value()["name"].get<std::string>();
						mBufferViews.push_back(bufferview);
					}

				}
				if ( strcmp(it.key().c_str(), "buffers") == 0)
				{
					printf("Buffers\n");
					auto buffers = it.value().items();
					// TODO varios buffers
					for (auto buff : buffers)
					{
						// byteLength
						if (!buff.value()["byteLength"].is_null())
							mBinaryLength = buff.value()["byteLength"].get<unsigned int>();
						else
							fprintf(stderr, "FAIL on reading glTF");
						// uri
						if (!buff.value()["uri"].is_null())
						{	// TODO _path.split("//"), quitamos la ultima parte que es la ruta del glTF
							auto uri = buff.value()["uri"].get<std::string>();
							char path[128];
							memcpy(path, _path.c_str(), 128);
							auto tokenize = strtok(path, ".");
							std::string tok(tokenize);
							tok += ".bin";
							mBinaryURI = tok;
						}
						else
							fprintf(stderr, "FAIL on reading glTF");
					}
				}
			}
		}
		else
		{
			fprintf(stderr, "Error en el gltf");
		}
	}
	mMaterial = new Material("basic_shader", "basic_shader", false, glm::vec3(1.f));
	//VAO = _CreateBuffers(mNodes, mNodes);
	ProcessScene();
#endif
	cgltf_options _options {cgltf_file_type_invalid};
	cgltf_data* data = NULL;
	cgltf_result result = cgltf_parse_file(&_options, _path.c_str(), &data);
	if (result == cgltf_result_success)
	{
		/* TODO make awesome stuff */
		cgltf_result resultBuffer  = cgltf_load_buffers(&_options, data, _path.c_str());
		cgltf_free(data);
	}
}

void cglTFFile::Draw(glm::mat4 view,
	glm::mat4 projection, glm::vec3 camera_position, glm::vec3 _lightPosition,
	glm::vec3 _lightColor)
{
}

void cglTFFile::ProcessScene()
{
	FILE* f = fopen(mBinaryURI.c_str(), "rb");
	if(f)
	{
		/*std::ifstream mBinFile;
		mBinFile = std::ifstream(mBinaryURI, std::ios::binary);*/
		size_t rPos = 0;
		for (auto mesh : mMeshes)
		{
			printf("Mesh: %s\n",mesh.mName.c_str());
			for( auto primitive : mesh.mPrimitives)
			{
				 
				printf("Primitive indice: %d\n",	primitive.mIndices);
				// SCALAR
				if(primitive.mIndices != -1)
				{
					mBinaryData = new char[mBufferViews[mAccessors[primitive.mIndices].mBufferView].mByteLength];
					fread(mBinaryData, mBufferViews[mAccessors[primitive.mIndices].mBufferView].mByteLength, 1, f);
					//mBinFile.read(mBinaryData, mBufferViews[mAccessors[primitive.mIndices].mBufferView].mByteLength);
					printf("\t\tBinaryData indice: %d\n", static_cast<unsigned int>(*mBinaryData));
					//fseek(f, mBufferViews[mAccessors[primitive.mIndices].mBufferView].mByteLength, static_cast<int>(rPos));
					if(mBinaryData != '\0')
						delete mBinaryData;
				}
			 
				//printf("\tPosition %s\n",		mBufferViews[mAccessors[primitive.mAttributes.mPosition].mBufferView].mName.c_str());
				// VEC3
				if(primitive.mAttributes.mPosition != -1)
				{
					mBinaryData = new char[mBufferViews[mAccessors[primitive.mAttributes.mPosition].mBufferView].mByteLength];
					fread(mBinaryData, mBufferViews[mAccessors[primitive.mAttributes.mPosition].mBufferView].mByteLength, 1, f);
					//mBinFile.read(mBinaryData, mBufferViews[mAccessors[primitive.mAttributes.mPosition].mBufferView].mByteLength);
					float position[3];
					position[0] = static_cast<float>(mBinaryData[0]);
					position[1] = static_cast<float>(mBinaryData[sizeof(float)]);
					position[2] = static_cast<float>(mBinaryData[2*sizeof(float)]);
					printf("\t\tBinaryData Position: (%f, %f, %f)\n", position[0], position[1], position[2]);
					if (mBinaryData != '\0')
						delete mBinaryData;
				}
			
				//printf("\tNormal %s\n", mBufferViews[mAccessors[primitive.mAttributes.mNormal].mBufferView].mName.c_str());
				// VEC3
				if(primitive.mAttributes.mNormal != -1)
				{
					mBinaryData = new char[mBufferViews[mAccessors[primitive.mAttributes.mNormal].mBufferView].mByteLength];
					fseek(f, mBufferViews[mAccessors[primitive.mAttributes.mNormal].mBufferView].mByteOffset, 0);
					fread(mBinaryData, mBufferViews[mAccessors[primitive.mAttributes.mNormal].mBufferView].mByteLength, 1, f);
					//mBinFile.read(mBinaryData, mBufferViews[mAccessors[primitive.mAttributes.mNormal].mBufferView].mByteLength);
					float normal[3];
					normal[0] = static_cast<float>(mBinaryData[0]);
					normal[1] = static_cast<float>(mBinaryData[sizeof(float)]);
					normal[2] = static_cast<float>(mBinaryData[2*sizeof(float)]);
					printf("\t\tBinaryData Normal: (%f, %f, %f)\n", normal[0], normal[1], normal[2]);
					if (mBinaryData != '\0')
						delete mBinaryData;
				}

				//printf("\tTangent %s\n", mBufferViews[mAccessors[primitive.mAttributes.mTangent].mBufferView].mName.c_str());
				// VEC4
				if(primitive.mAttributes.mTangent != -1)
				{
					mBinaryData = new char[mBufferViews[mAccessors[primitive.mAttributes.mTangent].mBufferView].mByteLength];
					fseek(f, mBufferViews[mAccessors[primitive.mAttributes.mTangent].mBufferView].mByteOffset, 0);
					fread(mBinaryData, mBufferViews[mAccessors[primitive.mAttributes.mTangent].mBufferView].mByteLength, 1, f);
					//mBinFile.read(mBinaryData, mBufferViews[mAccessors[primitive.mAttributes.mTangent].mBufferView].mByteLength);
					float tangent[4];
					tangent[0] = static_cast<float>(mBinaryData[0]);
					tangent[1] = static_cast<float>(mBinaryData[sizeof(float)]);
					tangent[2] = static_cast<float>(mBinaryData[2*sizeof(float)]);
					tangent[3] = static_cast<float>(mBinaryData[3*sizeof(float)]);
					printf("\t\tBinaryData Tangent: (%f, %f, %f, %f)\n", tangent[0], tangent[1], tangent[2], tangent[3]);
					if (mBinaryData != '\0')
						delete mBinaryData;
				}
				 
				//printf("\tUV0 %s\n", mBufferViews[mAccessors[primitive.mAttributes.mTexCoord0].mBufferView].mName.c_str());
				// VEC2
				if(primitive.mAttributes.mTexCoord0 != -1)
				{
					mBinaryData = new char[mBufferViews[mAccessors[primitive.mAttributes.mTexCoord0].mBufferView].mByteLength];
					fseek(f, mBufferViews[mAccessors[primitive.mAttributes.mTexCoord0].mBufferView].mByteOffset, 0);
					fread(mBinaryData, mBufferViews[mAccessors[primitive.mAttributes.mTexCoord0].mBufferView].mByteLength, 1, f);
					//mBinFile.read(mBinaryData, mBufferViews[mAccessors[primitive.mAttributes.mTexCoord0].mBufferView].mByteLength);
					float UV0[2];
					UV0[0] = static_cast<float>(mBinaryData[0]);
					UV0[1] = static_cast<float>(mBinaryData[sizeof(float)]);
					printf("\t\tBinaryData UV0: (%f, %f)\n", UV0[0], UV0[1]);
					if (mBinaryData != '\0')
						delete mBinaryData;
				}
				
				//printf("\tUV1 %s\n", mBufferViews[mAccessors[primitive.mAttributes.mTexCoord1].mBufferView].mName.c_str());
				// VEC2
				if (primitive.mAttributes.mTexCoord1 != -1)
				{
					mBinaryData = new char[mBufferViews[mAccessors[primitive.mAttributes.mTexCoord1].mBufferView].mByteLength];
					fseek(f, mBufferViews[mAccessors[primitive.mAttributes.mTexCoord1].mBufferView].mByteOffset, 0);
					fread(mBinaryData, mBufferViews[mAccessors[primitive.mAttributes.mTexCoord1].mBufferView].mByteLength, 1, f);
					//mBinFile.read(mBinaryData, mBufferViews[mAccessors[primitive.mAttributes.mTexCoord1].mBufferView].mByteLength);
					float UV1[2];
					UV1[0] = static_cast<float>(mBinaryData[0]);
					UV1[1] = static_cast<float>(mBinaryData[sizeof(float)]);
					printf("\t\tBinaryData UV1: (%f, %f)\n", UV1[0], UV1[1]);
					if (mBinaryData != '\0')
						delete mBinaryData;
				}
			}
			printf("\n");
		}
		fclose(f);
	}
}
