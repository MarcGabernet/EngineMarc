#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleTexture.h"

#include <locale>
#include <codecvt>

ModuleTexture::ModuleTexture()
{
}

// Destructor
ModuleTexture::~ModuleTexture()
{
}

// Called before render is available
bool ModuleTexture::Init()
{
	//LoadFile("baboon.ppm");
	//texture_id = GenerateTexture();

	return true;
}

// Called before quitting
bool ModuleTexture::CleanUp()
{
	glDeleteBuffers(1, &texture_id);

	return true;
}

void ModuleTexture::LoadFile(std::string fileName)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring wstr = converter.from_bytes(fileName);

	// Access the const wchar_t* pointer
	const wchar_t* fileN = wstr.c_str();

	HRESULT hr = DirectX::LoadFromDDSFile(fileN, DirectX::DDS_FLAGS_NONE, &info, image);
	if (FAILED(hr))
	{
		hr = DirectX::LoadFromTGAFile(fileN, &info, image);
	}
	if (FAILED(hr))
	{
		hr = DirectX::LoadFromWICFile(fileN, DirectX::WIC_FLAGS_NONE, &info, image);
	}
	if (FAILED(hr))
	{
		LOG("TEXTURE LOADING FALIED");
	}
	width = image.GetMetadata().width;
	height = image.GetMetadata().height;

}

unsigned ModuleTexture::GenerateTexture() 
{
	unsigned texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//	Wrapping/Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	CheckType();

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, image.GetPixels());
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(texture);

	return texture;
}

void ModuleTexture::CheckType()
{
	switch (image.GetMetadata().format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		internalFormat = GL_RGB8;
		format = GL_BGR;
		type = GL_UNSIGNED_BYTE;
		break;
	default:
		assert(false && "Unsupported format");
	}
}
