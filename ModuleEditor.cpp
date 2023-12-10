#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleCamera.h"

#include "MathBuildConfig.h"
#include "MathGeoLib.h"
#include "MathGeoLibFwd.h"

ModuleEditor::ModuleEditor()
{
}

// Destructor
ModuleEditor::~ModuleEditor()
{
}

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

bool ModuleEditor::Init() 
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	SDL_GL_MakeCurrent(App->GetWindow()->window, App->GetOpenGL()->context);
	ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->context);
	ImGui_ImplOpenGL3_Init("#version 460");

	return true;
}

update_status ModuleEditor::PreUpdate()
{

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	InitialWindow();

	if (show_camera_window)
	{
		Camera_Window();
	}

	RenderAndUpdate();
	
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}


void ModuleEditor::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->GetWindow()->window);
	ImGui::NewFrame();
}
void ModuleEditor::RenderAndUpdate() 
{
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
	SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
}

//Pop-up Windows
void ModuleEditor::InitialWindow()
{
	static float h_fov = 0.0f;
	static int counter = 0;

	ImGui::Begin("Initial Application Window");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Checkbox("Demo Examples Window", &show_demo_window);      // Edit bools storing our window open/close state

	ImGui::Text(" ");
	ImGui::Checkbox("Camera Properties Window", &show_camera_window);

	ImGui::Text(" ");
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

	ImGui::End();
}
void ModuleEditor::Camera_Window()
{
	//Target
	static float target[3] = {App->GetCamera()->target.x,  App->GetCamera()->target.y, App->GetCamera()->target.z};

	//Position
	static float position[3] = { App->GetCamera()->GetPosition().x,  App->GetCamera()->GetPosition().y, App->GetCamera()->GetPosition().z};

	//Field of view
	static float h_fov = App->GetCamera()->GetFrustum()->horizontalFov;
	static float h_fov_deg;
	//Aspect 
	static float aspect = App->GetCamera()->GetAspect();
	
	//Planes
	static float nearPlane = App->GetCamera()->GetFrustum()->nearPlaneDistance;
	static float farPlane = App->GetCamera()->GetFrustum()->farPlaneDistance;

	//Speed
	static float movementSpeed = App->GetCamera()->GetMovementSpeed();
	static float rotationSpeed = App->GetCamera()->GetRotationSpeed();

	ImGui::Begin("Camera Properties Editor", &show_camera_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	
	ImGui::Checkbox("Orbiting", &App->GetCamera()->orbiting);
	if (App->GetCamera()->orbiting) 
	{
		ImGui::InputFloat3("Target", target);

		App->GetCamera()->target = float3(target[0], target[1], target[2]);
	}

	ImGui::SeparatorText("Transform");

		position[0] = App->GetCamera()->GetPosition().x;
		position[1] = App->GetCamera()->GetPosition().y;
		position[2] = App->GetCamera()->GetPosition().z;

		ImGui::InputFloat3("Position", position);
		App->GetCamera()->SetPosition(float3(position[0], position[1], position[2]));

	ImGui::SeparatorText("View");

	h_fov_deg = h_fov * 180.0 / math::pi;
	ImGui::SliderFloat("Field of View", &h_fov_deg, 0.0f, 180);
	h_fov = h_fov_deg * math::pi / 180.0f;
	ImGui::SliderFloat("Aspect Ratio", &aspect, 0.25f, 4.0f);
	App->GetCamera()->ChangeFOV(aspect, h_fov, App->GetWindow()->width, App->GetWindow()->height);

	ImGui::SeparatorText("Plane Distances");
	ImGui::SliderFloat("Near plane", &nearPlane, 0.001f, farPlane);
	ImGui::SliderFloat("Far plane", &farPlane, nearPlane, 200.0);
	App->GetCamera()->GetFrustum()->SetPlaneDistances(nearPlane, farPlane);

	ImGui::SeparatorText("Speed");
	ImGui::SliderFloat("Movement", &movementSpeed, 1.0f, 20.0f);
	ImGui::SliderFloat("Rotation", &rotationSpeed, 0.2f, 4.0f);
	App->GetCamera()->SetSpeeds(movementSpeed, rotationSpeed); 

	ImGui::Text("");
	if (ImGui::Button("Reset Camera Properties"))
	{
		//View
		h_fov = math::pi / 4.0;
		aspect = 1;
		//Planes
		nearPlane = 0.001f;
		farPlane = 100.0f;
		//Speed
		movementSpeed = 5.0f;
		rotationSpeed = 1.0f;
	}

	ImGui::End();
}
void ModuleEditor::DebugWindowSize(int widht, int height)
{
	ImGui::Begin("Window size Debugger");

	ImGui::Text("W: , %d", widht);
	ImGui::Text("H: , %d", height);

	ImGui::End();
}