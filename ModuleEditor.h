#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "Globals.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor(); 
	~ModuleEditor(); 

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void DebugWindowSize(int widht, int height);

	void NewFrame();
	void RenderAndUpdate();

private:
	void InitialWindow();
	void Camera_Window();

	bool show_demo_window = false;
	bool done = false;
	bool show_camera_window = true;

};

#endif