#include "Application.h"
#include "Model.h"
#include "Mesh.h"
#include "ModuleTexture.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION // Only in one of the includes 
#include "tiny_gltf.h"

Model::Model()
{
	ChangeTransform(position, rotation, scale);
}

// Destructor
Model::~Model()
{
}

void Model::Load(const char* assetFileName)
{
	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model sourceModel;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&sourceModel, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
	}

	for (const auto& srcMesh : sourceModel.meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			Mesh* mesh = new Mesh;
			mesh->Load(sourceModel, srcMesh, primitive);
			meshes.push_back(mesh);
		}
	}
	LoadMaterials(sourceModel);
}

void Model::LoadMaterials(const tinygltf::Model& srcModel)
{
	for (const auto& srcMaterial : srcModel.materials)
	{
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			App->GetTexture()->LoadFile("./Models/"+image.uri);
			textureId = App->GetTexture()->GenerateTexture();
		}
		textures.push_back(textureId);
	}
}

void Model::Draw()
{

	for (const auto& mesh : meshes)
	{
		mesh->Draw(textures);
	}

}

bool Model::CleanUp()
{

	for (const auto& mesh : meshes)
	{
		mesh->DeleteBuffers();
	}
	meshes.clear();
	textures.clear();

	return true;
}

void Model::ChangeTransform(float3 position, float rotation, float3 scale)
{
	model = float4x4::FromTRS(position,
		float4x4::RotateZ(rotation),
		scale);
}