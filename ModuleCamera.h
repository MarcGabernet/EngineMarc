#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Module.h"
#include "Globals.h"
#include <cmath>

#include "SDL/include/SDL.h"

#include "MathBuildConfig.h"
#include "MathGeoLib.h"
#include "MathGeoLibFwd.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera(float aspect, Frustum frustum);
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	//Position
	float3 GetPosition() { return position; }
	void SetPosition(float3 position) { frustum.pos = position; }

	Frustum* GetFrustum() { return &frustum; }
	const float GetAspect() const { return aspect; }

	//Speeds
	const float GetMovementSpeed() const { return movementSpeed; }
	const float GetRotationSpeed() const { return rotationSpeed; }
	void SetSpeeds(float movement, float rotation) 
	{ 
		movementSpeed = movement;
		rotationSpeed = rotation;
	}

	//Matrices
	float4x4 GetProjectionMatrix() const { return proj; }
	float4x4 GetViewMatrix() const { return view; }

	//Field of view
	void ChangeFOV(float aspect, float horizontalFOV, int width, int height);
	//Camera vectors
	void RecalculateUpFrontVectors(float3x3 rotationMatrix);

	//Orbital camera
	bool orbiting = false;
	float3 target = float3::zero;

	//Mouse input
	void MouseMovement(SDL_MouseMotionEvent motion);
	void MouseWheelMovement(SDL_Event event);

private:

	//Transform
	float3 position;
	
	//Frustum
	Frustum frustum;

	//Camera aspect
	float aspect;

	//Camera movement speed
	float movementSpeed = 5;
	//Camera movement speed
	float rotationSpeed = 1;

	//Matrices
	float4x4 proj = float4x4::identity;
	float4x4 view = float4x4::identity;

	//ROTATION MATRIX: Functions used to simplify the construction of the general Rotation Matrix
	const float DiagonalElement(const float axisDimension, const float delta) const
	{
		return Cos(delta) + Pow(axisDimension, 2) * (1 - Cos(delta));
	}
	const float NonDiagonalElement(const float axisDim1, const float axisDim2, const float axisDim3, const float delta, const int sign) const
	{
		return axisDim1 * axisDim2 * (1 - Cos(delta)) + sign * axisDim3 * Sin(delta);
	}
	const float3x3 RotationMatrix(const float3 axis, const float delta) const 
	{
		return float3x3(	float3(	DiagonalElement(axis.x, delta),
									NonDiagonalElement(axis.x, axis.y, axis.z, delta, 1),
									NonDiagonalElement(axis.x, axis.z, axis.y, delta, -1)),

							float3(	NonDiagonalElement(axis.x, axis.y, axis.z, delta, -1),
									DiagonalElement(axis.y, delta),
									NonDiagonalElement(axis.y, axis.z, axis.x, delta, 1)),

							float3(	NonDiagonalElement(axis.x, axis.z, axis.y, delta, 1),
									NonDiagonalElement(axis.y, axis.z, axis.x, delta, -1),
									DiagonalElement(axis.z, delta)));
	}

	//LookAt Funcition
	void LookAt(float3 cameraPos, float3 target);

	//Orbiting movement
	void OribitingMovement(float3 target, float speed);

	//Keybard Input
	void MovementInput(float speed);
	void RotationInput(float speed);

};

#endif
