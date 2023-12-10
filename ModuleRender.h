#ifndef __ModuleRender_H__
#define __ModuleRender__H__

#include "Module.h"
#include "Globals.h"
#include "Model.h"

#include <string>

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	Model* GetModel(){ return model; }

	void ChangeModel(std::string fileName);

	unsigned vbo_id = 0;

private:
	unsigned BaboonVbo();
	void RenderBaboon(unsigned vbo, unsigned program);

	Model* model;

	float random = 0;
	float incr = 1;
};

#endif

