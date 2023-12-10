#ifndef __Model_H__
#define __Model_H__

#include "Module.h"
#include "Globals.h"

#include <vector>

class Mesh;

namespace tinygltf
{
	class Model;
	class Mesh;
	class Primitive;
}

class Model
{
public:
	Model();
	~Model();
	void Load(const char* assetFileName);
	void Draw();
	void DeleteBuffers();

private:
	void LoadMaterials(const tinygltf::Model& srcModel);

	//float4x4 model = float4x4::identity * float4x4(float3x3::identity * 50);

	std::vector<Mesh*> meshes;
	std::vector<unsigned> textures;
};

#endif
