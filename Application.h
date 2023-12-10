#ifndef __Application_H__
#define __Application_H__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleRender;
class ModuleEditor; 
class ModuleInput;
class ModuleProgram;
class ModuleCamera;
class ModuleDebugDraw;
class ModuleTexture;
class Model;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleWindow* GetWindow() { return window; }
    ModuleProgram* GetProgram() { return program; }
    ModuleCamera* GetCamera() { return camera; }
    ModuleDebugDraw* GetDebug() { return debug; }
    ModuleEditor* GetEditor() { return editor; }
    ModuleInput*  GetInput() { return input; }
    ModuleTexture* GetTexture() { return texture; }

    ModuleRender* GetExercise() { return exercise; }

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleProgram* program = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleDebugDraw* debug = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleInput* input = nullptr;
    ModuleTexture* texture = nullptr;
    ModuleRender* exercise = nullptr;

    std::list<Module*> modules;

};

extern Application* App;

#endif