#pragma once
#include "../../nlohmann/json.hpp"
#include "glm.hpp"
#include "Material.h"

//#include "RaiEng.h"

struct sNode
{
	unsigned int mMesh;
	unsigned int mCamera;
	glm::mat4 mModel {1.f};
	std::string mName;
	std::vector<float>	mTranslation;
	std::vector<float>	mRotation;
	std::vector<float>	mScale;
	std::vector<int>	mChildren;
	std::vector<float>	mMatrix;

};

struct sPrimitiveAttribute
{
	unsigned int mPosition = -1;
	unsigned int mTangent = -1;
	unsigned int mNormal = -1;
	unsigned int mTexCoord0 = -1;
	unsigned int mTexCoord1 = -1;
};
struct sMeshPrimitive
{
	unsigned int mIndices;
	unsigned int mMaterial;
	sPrimitiveAttribute mAttributes;
};
struct sMesh
{
	std::string mName;
	std::vector<sMeshPrimitive> mPrimitives;
};
struct sAccessor
{
	unsigned int mBufferView;
	unsigned int mComponentType;
	unsigned int mByteOffset;
	unsigned int mCount;
	std::string mType;
	std::string mName;
	std::vector<float> mMax;
	std::vector<float> mMin;
};

struct sBufferView
{
	unsigned int mBuffer;
	unsigned int mTarget;
	unsigned int mByteLength;
	unsigned int mByteStride;
	unsigned int mByteOffset;
	std::string  mName;
};

struct sScene
{
	std::vector<unsigned int> mNodes;
};
// Model -> Nodes -> Meshes -> Vertices
//							-> Faces -> indices
class cglTFFile
{
	char* mBinaryData;
	std::string mBinaryURI;
	unsigned int mBinaryLength;
	unsigned int VAO;
	int mScene;
	std::vector<sScene> mScenes;
	std::vector<unsigned int> mIndices;
	//std::vector<Vertex> mVertices;

	std::vector<sNode> mNodes;
	std::vector<sMesh> mMeshes;
	std::vector<sAccessor> mAccessors;
	std::vector<sBufferView> mBufferViews;
	Material* mMaterial;
public:
	cglTFFile(std::string _path);
	void Draw(glm::mat4 view,
		glm::mat4 projection, glm::vec3 camera_position, glm::vec3 _lightPosition,
		glm::vec3 _lightColor);
private:
	void ProcessScene();
};
