#ifndef __Mesh_H__
#define __Mesh_H__

#include "Module.h"
#include "Globals.h"
#include <vector>

namespace tinygltf 
{
	class Model;
	class Mesh;
	class Primitive;
	class Accessor;
}

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Load(const tinygltf::Model sourceModel, const tinygltf::Mesh srcMesh, const tinygltf::Primitive primitive);
	void Draw(const std::vector<unsigned>& textures);

	unsigned vbo = 0;
	unsigned tex = 0;
	unsigned ebo = 0;
	unsigned vao = 0;

	void  DeleteBuffers();

private:

	int vertexCount;
	int indexCount;
	int materialIndex;

	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void CreateVAO();

};

#endif
