#ifndef __ModuleTexture_H__
#define __ModuleTexture_H__

#include "Module.h"
#include "Globals.h"

#include "DirectXTex.h"
#include "glew-2.1.0/include/GL/glew.h"
#include <string>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	bool CleanUp();

	unsigned texture_id;

	void LoadFile(std::string fileName);
	unsigned GenerateTexture();

private:
	void CheckType();

	DirectX::TexMetadata info;
	DirectX::ScratchImage image;

	int width;
	int height;

	GLint internalFormat;
	GLenum format;
	GLenum type;
};

#endif