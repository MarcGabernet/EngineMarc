#include "Application.h"
#include "ModuleInput.h"
#include "ModuleOpenGL.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"


ModuleInput::ModuleInput()
{
	keys = new KeyState[MAX_KEYS];
	memset(keys, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    if (sdlEvent.window.windowID == SDL_GetWindowID(App->GetWindow()->window)) 
                    {
                        App->GetOpenGL()->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                        App->GetCamera()->ChangeFOV(App->GetCamera()->GetAspect(), App->GetCamera()->GetFrustum()->horizontalFov, App->GetWindow()->width, App->GetWindow()->height);
                    }
                break;
			case SDL_KEYDOWN:
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				MouseKeysPressed(sdlEvent);
				break;
			case SDL_MOUSEWHEEL:
				App->GetCamera()->MouseWheelMovement(sdlEvent);
				break;
			case SDL_MOUSEMOTION:
				App->GetCamera()->MouseMovement(sdlEvent.motion);
				break;
			case SDL_DROPFILE: 
				char* filePath = sdlEvent.drop.file;
				App->GetExercise()->ChangeModel(filePath);
				SDL_free(filePath);
				break;
        }
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
    }

    keyboard = SDL_GetKeyboardState(NULL);
	GetKeysPressed(keyboard);

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput:: MouseKeysPressed(SDL_Event event) 
{
	switch (event.type) 
	{
	case SDL_MOUSEBUTTONDOWN:
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			mouseLeftPressed = true;
			break;
		case SDL_BUTTON_RIGHT:
			mouseRightPressed = true;
			break;
		case SDL_BUTTON_MIDDLE:
			mouseMiddlePressed = true;
			break;
		}
		break;

	case SDL_MOUSEBUTTONUP:
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			mouseLeftPressed = false;
			break;
		case SDL_BUTTON_RIGHT:
			mouseRightPressed = false;
			break;
		case SDL_BUTTON_MIDDLE:
			mouseMiddlePressed = false;
			break;
		}
		break;
	}
}

void ModuleInput::GetKeysPressed(const Uint8* keyboard)
{
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i] == 1)
		{
			if (keys[i] == KEY_IDLE)
				keys[i] = KEY_DOWN;
			else
				keys[i] = KEY_REPEAT;
		}
		else
		{
			if (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN)
				keys[i] = KEY_UP;
			else
				keys[i] = KEY_IDLE;
		}
	}
}