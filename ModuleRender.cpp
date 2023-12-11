#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleRender.h"
#include "Model.h"


ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	//vbo_id = BaboonVbo();
	model = new Model;
	model->Load("./Models/BakerHouse/BakerHouse.gltf");

	return true;
}

update_status ModuleRender::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	//RenderBaboon(vbo_id, App->GetProgram()->program);
	model->Draw();

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying program");

	glDeleteBuffers(1, &vbo_id);
	model->CleanUp();

	return true;
}

// BAOBOON: This function must be called one time at creation of vertex buffer
unsigned ModuleRender::BaboonVbo()
{
	float vtx_data[] =	{	 //Triangle 1
							 -1,	-1,		0,
							  1,	-1,		0,
							 -1,	 1,		0,
							//Triangle 2
							  1,	-1,		0,
							  1,	 1,		0,
							 -1,	 1,		0,
							 //Coords 1
							0.0f,	0.0f,
							1.0f,	0.0f,
							0.0f,	1.0f,
							//Coords 2
							1.0f,	0.0f,
							1.0f,	1.0f,
							0.0f,	1.0f
	};
	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_DYNAMIC_DRAW);

	return vbo;
}
void ModuleRender::RenderBaboon(unsigned vbo, unsigned program)
{
	glUseProgram(program);

	glUniformMatrix4fv(2, 1, GL_TRUE, &App->GetExercise()->GetModel()->GetModelMatrix()[0][0]);
	glUniformMatrix4fv(3, 1, GL_TRUE, &App->GetCamera()->GetViewMatrix()[0][0]);
	glUniformMatrix4fv(4, 1, GL_TRUE, &App->GetCamera()->GetProjectionMatrix()[0][0]);

	// Bind buffer and vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 18));

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, App->GetTexture()->texture_id);

	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ModuleRender::ChangeModel(std::string fileName) 
{
	int last = fileName.find_last_of('\\');
	std::string name = "";
	int len = fileName.length();
	for (int i = last + 1; i < fileName.length() -5; i++)
	{
		name += fileName[i];
	}
	model->CleanUp();

	std::string path = "./Models/" + name + "/" + name + ".gltf";
	const char* filePath = path.c_str();
	model->Load(filePath);
}