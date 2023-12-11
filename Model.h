#ifndef __Model_H__
#define __Model_H__

#include "Module.h"
#include "Globals.h"

#include <vector>
#include <string>

#include "MathBuildConfig.h"
#include "MathGeoLib.h"
#include "MathGeoLibFwd.h"

class Mesh;

namespace tinygltf
{
	class Model;
	class Mesh;
	class Primitive;
}

class Model //: public Module
{
public:
	//bool Init();
	//update_status Update();
	bool CleanUp();

	Model();
	~Model();
	void Load(const char* assetFileName);
	void Draw();

	//Transform & matrix
	float4x4 GetModelMatrix() { return model; }

	void ChangePosition(float3 position);
	void ChangeRotation(float rotation);
	void ChangeScale(float3 scale);
	void  ChangeTransform(float3 position, float rotation, float3 scale);

	const float3 GetPosition() { return position; }
	const float GetRotation() { return rotation; }
	const float3 GetScale() { return scale; }

	void SetPosition(float3 pos) { position = pos; }
	void SetPosition(float rot) { rotation = rot; }
	void SetScale(float3 scl) { scale = scl; }

	//Properties
	int GetVertexCount() { return vertexCount; }
	int GetTriangleCount() { return indexCount/3; }

	//File Name
	const std::string GetFileName() { return fileName; }

	//Geometry
	std::vector<Mesh*> GetMeshes() const { return meshes; }

private:
	void LoadMaterials(const tinygltf::Model& srcModel);

	std::string fileName;

	//Transform & matrix
	float4x4 model;
	float3 position = float3::zero;
	float rotation = 0;
	float3 scale = float3::one;

	//Properties
	int vertexCount = 0;
	int indexCount = 0;

	//Geometry & textures
	std::vector<Mesh*> meshes;
	std::vector<unsigned> textures;
};

#endif
