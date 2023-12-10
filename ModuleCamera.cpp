#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

ModuleCamera::ModuleCamera(float aspect, Frustum frust)
{
	frustum = frust;

	this->aspect = aspect;
	this->position = frustum.pos;

	frustum.SetAspectRatio(aspect, SCREEN_WIDTH, SCREEN_HEIGHT);
}

// Destructor
ModuleCamera::~ModuleCamera()
{
}

// Called before render is available
bool ModuleCamera::Init()
{
	proj = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();

	return true;
}

update_status ModuleCamera::PreUpdate()
{

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{
	static Uint32 lastframeTime = SDL_GetTicks();
	Uint32 currentFrame = SDL_GetTicks();
	float deltaTime = (currentFrame - lastframeTime) / 1000.0f; //To seconds

	if (orbiting)
	{
		OribitingMovement(target, deltaTime * rotationSpeed);
	}
	else 
	{
		//Input
		MovementInput(deltaTime * movementSpeed);
		RotationInput(deltaTime * rotationSpeed);
	}
	lastframeTime = currentFrame;

	//Update position
	position = frustum.pos;

	//Update view matrix
	view = frustum.ViewMatrix();

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleCamera::CleanUp()
{

	return true;
}

void ModuleCamera::ChangeFOV(float aspect, float horizontalFOV, int width, int height) 
{
	this->aspect = aspect;

	frustum.SetFOV(horizontalFOV);
	frustum.SetAspectRatio(aspect, width, height);

	proj = frustum.ProjectionMatrix();
}

void ModuleCamera::RecalculateUpFrontVectors(float3x3 rotationMatrix)
{
	float3 oldFront = frustum.front.Normalized();
	float3 oldUp = frustum.up.Normalized();

	float3 newUp = rotationMatrix.MulDir(oldUp);

	if (newUp.y >= 0)
	{
		frustum.SetFront(rotationMatrix.MulDir(oldFront));
		frustum.SetUp(newUp);
	}
}

//Orbiting
void ModuleCamera::LookAt(float3 cameraPos, float3 target)
{
	//Only if target and position are not equal
	if(cameraPos.x != target.x || cameraPos.y != target.y || cameraPos.z != target.z)
	{
		//Set front
		float3 front = target - cameraPos;
		frustum.front = front.Normalized();

		float3 up;
		//In case unitY and front do not generate a base we manually select a up vector
		if (Abs(front.y) == 1)
		{
			up = float3(0, 0, front.y);
		}
		else
		{
			//Set right
			float3 right = front.Cross(float3::unitY);

			//Set up
			up = right.Cross(front);
			frustum.up = up.Normalized();
		}
		frustum.pos = cameraPos;
	}

}
void ModuleCamera::OribitingMovement(float3 target, float speed) 
{
	//Speed controller
	if (App->GetInput()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 2;

	float3 radius = frustum.pos - target;
	float3 sphericalCoords = radius.ToSphericalCoordinates(); //azimuth, inclination, length

	if (App->GetInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->GetInput()->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		sphericalCoords.x += speed;
	if (App->GetInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->GetInput()->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		sphericalCoords.x -= speed;
	if (App->GetInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || App->GetInput()->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		sphericalCoords.y -= speed;
	if (App->GetInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->GetInput()->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		sphericalCoords.y += speed;
	//float3 newPos = frustum.pos;
	float3 newPos = target + sphericalCoords.FromSphericalCoordinates(sphericalCoords.x, sphericalCoords.y, sphericalCoords.z);

	if (newPos.x != frustum.pos.x || newPos.y != frustum.pos.y || newPos.z != frustum.pos.z)
		LookAt(newPos, target);
}

//Keyboard input
void ModuleCamera::MovementInput(float speed)
{
	//Speed controller
	if (App->GetInput()->GetKey(SDL_SCANCODE_LSHIFT ) == KEY_REPEAT)
		speed *= 2;

	float3 increase = float3::zero;

	if (App->GetInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		increase += frustum.WorldRight();
	if (App->GetInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		increase -= frustum.WorldRight();
	if (App->GetInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		increase += float3::unitY.Cross(frustum.WorldRight());
	if (App->GetInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		increase -= float3::unitY.Cross(frustum.WorldRight());
	if (App->GetInput()->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		increase += float3(0, 1, 0);
	if (App->GetInput()->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		increase -= float3(0, 1, 0);

	increase *= speed;
	frustum.pos += increase;
}
void ModuleCamera::RotationInput(float speed)
{
	//Speed controller
	if (App->GetInput()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 2;

	if (App->GetInput()->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		RecalculateUpFrontVectors(float3x3::RotateAxisAngle(frustum.WorldRight(), speed));
	if (App->GetInput()->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		RecalculateUpFrontVectors(float3x3::RotateAxisAngle(frustum.WorldRight(), -speed));
	if (App->GetInput()->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		RecalculateUpFrontVectors(float3x3::RotateAxisAngle(float3::unitY, speed));
	if (App->GetInput()->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		RecalculateUpFrontVectors(float3x3::RotateAxisAngle(float3::unitY, -speed));
	//if (App->GetInput()->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	//	RecalculateUpFrontVectors(float3x3::RotateAxisAngle(frustum.front, speed));
	//if (App->GetInput()->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	//	RecalculateUpFrontVectors(float3x3::RotateAxisAngle(frustum.front, -speed));
}

//Mouse input
void ModuleCamera::MouseMovement(SDL_MouseMotionEvent motion)
{
	static const float speed = 0.001f;
	static bool lastMousePressed = App->GetInput()->mouseRightPressed;

	if (App->GetInput()->mouseRightPressed && !orbiting)
	{
		static int lastMousePos_x = motion.x;
		static int lastMousePos_y = motion.y;
		if (!lastMousePressed) 
		{
			lastMousePos_x = motion.x;
			lastMousePos_y = motion.y;
		}
		int currentMousePos_x = motion.x;
		int currentMousePos_y = motion.y;

		RecalculateUpFrontVectors(float3x3::RotateAxisAngle(float3::unitY, (currentMousePos_x - lastMousePos_x) * speed));
		RecalculateUpFrontVectors(float3x3::RotateAxisAngle(frustum.WorldRight(), (currentMousePos_y - lastMousePos_y) * speed));

		lastMousePos_x = currentMousePos_x;
		lastMousePos_y = currentMousePos_y;
	}
	lastMousePressed = App->GetInput()->mouseRightPressed;
}
void ModuleCamera::MouseWheelMovement(SDL_Event event) 
{
	if(!(orbiting && (frustum.pos - target).Length() <= event.wheel.y))
		frustum.pos += frustum.front * event.wheel.y/2.0;
}