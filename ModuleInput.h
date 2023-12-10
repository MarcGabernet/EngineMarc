#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"

#define MAX_KEYS 300

typedef unsigned __int8 Uint8;

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP,
};

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

	KeyState GetKey(int id) const 
	{ return keys[id]; }

	//Mouse
	int mousePosition_x;
	int mousePosition_y;

	bool mouseRightPressed = false;
	bool mouseMiddlePressed = false;
	bool mouseLeftPressed = false;

private:

	const Uint8 *keyboard = NULL;

	KeyState* keys;

	void GetKeysPressed(const Uint8* keyboard);
	void MouseKeysPressed(SDL_Event event);
};