#include "Application.h"
#include "Mesh.h"

#include "ModuleProgram.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"

#include "SDL.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "MathBuildConfig.h"
#include "MathGeoLib.h"
#include "MathGeoLibFwd.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"

Mesh::Mesh()
{
}

// Destructor
Mesh::~Mesh()
{
}

void Mesh::Load(const tinygltf::Model model, const tinygltf::Mesh srcMesh, const tinygltf::Primitive primitive)
{
	const auto& itPos = primitive.attributes.find("POSITION");
	const auto& itTex = primitive.attributes.find("TEXCOORD_0");
	if (itPos != primitive.attributes.end())
	{
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
		//vertexCount definition
		vertexCount = posAcc.count;
		materialIndex = primitive.material; 

		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);
		
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertexCount, nullptr, GL_STATIC_DRAW);

		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		size_t stride = posView.byteStride;
		if (stride == 0)
		{
			stride = sizeof(float) * 3;
		}
		for (size_t i = 0; i < posAcc.count; ++i)
		{
			ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			bufferPos += stride;
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	if (itTex != primitive.attributes.end())
	{
		const tinygltf::Accessor& texAcc = model.accessors[itTex->second];

		SDL_assert(texAcc.type == TINYGLTF_TYPE_VEC2);
		SDL_assert(texAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& texView = model.bufferViews[texAcc.bufferView];
		const tinygltf::Buffer& texBuffer = model.buffers[texView.buffer];
		const unsigned char* bufferTex = &(texBuffer.data[texAcc.byteOffset + texView.byteOffset]);

		glGenBuffers(1, &tex);
		glBindBuffer(GL_ARRAY_BUFFER, tex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * vertexCount, nullptr, GL_STATIC_DRAW);

		float2* ptr = reinterpret_cast<float2*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		
		size_t stride = texView.byteStride;
		if (stride == 0)
		{
			stride = sizeof(float) * 2;
		}
		for (size_t i = 0; i < texAcc.count; ++i)
		{
			ptr[i] = *reinterpret_cast<const float2*>(bufferTex);
			bufferTex += stride;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	LoadEBO(model, srcMesh, primitive);
	CreateVAO();

	meshName = srcMesh.name;
}

void Mesh::LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	if (primitive.indices >= 0)
	{
		const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
		indexCount = indAcc.count;
		const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];
		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset + indView.byteOffset]);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (uint32_t i = 0; i < indexCount; ++i)
			{
				ptr[i] = bufferInd[i];
			}
		}
		else if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		{
			const uint16_t* bufferInd = reinterpret_cast<const uint16_t*>(buffer);
			for (uint16_t i = 0; i < indexCount; ++i)
			{
				ptr[i] = bufferInd[i];
			}
		}
		else if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE)
		{
			const uint8_t* bufferInd = reinterpret_cast<const uint8_t*>(buffer);
			for (uint8_t i = 0; i < indexCount; ++i)
			{
				ptr[i] = bufferInd[i];
			}
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, tex);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);
}

void Mesh::Draw(const std::vector<unsigned>& textures)
{
	glUseProgram(App->GetProgram()->program);

	glUniformMatrix4fv(2, 1, GL_TRUE, &App->GetExercise()->GetModel()->GetModelMatrix()[0][0]);
	glUniformMatrix4fv(3, 1, GL_TRUE, &App->GetCamera()->GetViewMatrix()[0][0]);
	glUniformMatrix4fv(4, 1, GL_TRUE, &App->GetCamera()->GetProjectionMatrix()[0][0]);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, textures[materialIndex]);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void Mesh::DeleteBuffers()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &tex);
	glDeleteBuffers(1, &vao);
}