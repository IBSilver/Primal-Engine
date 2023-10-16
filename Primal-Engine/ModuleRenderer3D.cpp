#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

#ifdef _DEBUG
#pragma comment (lib, "MathGeoLib/libx86/Debug/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#else
#pragma comment (lib, "MathGeoLib/libx86/Release/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#endif // _DEBUG


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

static const GLfloat CubeVertices[] = {
	-1, -1, -1,
	1, -1, -1,
	1, 1, -1,
	-1, 1, -1,
	-1, -1, 1,
	1, -1, 1,
	1, 1, 1,
	-1, 1, 1
};
static const GLuint CubeIndices[] = {
	0, 1, 3, 3, 1, 2,
	1, 5, 2, 2, 5, 6,
	5, 4, 6, 6, 4, 7,
	4, 0, 7, 7, 0, 3,
	3, 2, 7, 7, 2, 6,
	4, 5, 0, 0, 5, 1
};// Called before render is available
bool ModuleRenderer3D::Init()
{
	//App->editor->Init();
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glewInit();
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);
	Grid.axis = true;

	VBO = 0;
	EBO = 0;
	//VAO = 0;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//glGenVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glBindVertexArray(VAO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glBindVertexArray(0);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());
	
	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();
	//DrawCube();
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	Grid.Render();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	ExitApp = App->editor->DrawEditor();

	SDL_GL_SwapWindow(App->window->window);
	if (ExitApp == false) {
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(ProjectionMatrix.M);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DrawCube()
{
	GLfloat Vertices[16][3] =
	{   // x     y     z
		{-1, -1,  1}, // 1  left    First Strip
		{-1.0,  1.0,  1.0}, // 3
		{-1.0, -1.0, -1.0}, // 0s
		{-1.0,  1.0, -1.0}, // 2
		{ 1.0, -1.0, -1.0}, // 4  back
		{ 1.0,  1.0, -1.0}, // 6
		{ 1.0, -1.0,  1.0}, // 5  right
		{ 1.0,  1.0,  1.0}, // 7
		{ 1.0,  1.0, -1.0}, // 6  top     Second Strip
		{-1.0,  1.0, -1.0}, // 2
		{ 1.0,  1.0,  1.0}, // 7
		{-1.0,  1.0,  1.0}, // 3
		{ 1.0, -1.0,  1.0}, // 5  front
		{-1.0, -1.0,  1.0}, // 1
		{ 1.0, -1.0, -1.0}, // 4  bottom
		{-1.0, -1.0, -1.0}  // 0
	};

	glBegin(GL_TRIANGLES);  // draw a cube 

	glVertex3fv(&Vertices[0][0]);
	glVertex3fv(&Vertices[1][0]);
	glVertex3fv(&Vertices[2][0]);

	glVertex3fv(&Vertices[2][0]);
	glVertex3fv(&Vertices[3][0]);
	glVertex3fv(&Vertices[1][0]);

	glVertex3fv(&Vertices[2][0]);
	glVertex3fv(&Vertices[3][0]);
	glVertex3fv(&Vertices[4][0]);

	glVertex3fv(&Vertices[4][0]);
	glVertex3fv(&Vertices[5][0]);
	glVertex3fv(&Vertices[3][0]);

	glVertex3fv(&Vertices[4][0]);
	glVertex3fv(&Vertices[5][0]);
	glVertex3fv(&Vertices[6][0]);

	glVertex3fv(&Vertices[5][0]);
	glVertex3fv(&Vertices[6][0]);
	glVertex3fv(&Vertices[7][0]);

	glVertex3fv(&Vertices[7][0]);
	glVertex3fv(&Vertices[8][0]);
	glVertex3fv(&Vertices[9][0]);

	glVertex3fv(&Vertices[1][0]);
	glVertex3fv(&Vertices[7][0]);
	glVertex3fv(&Vertices[9][0]);

	glVertex3fv(&Vertices[0][0]);
	glVertex3fv(&Vertices[2][0]);
	glVertex3fv(&Vertices[4][0]);

	glVertex3fv(&Vertices[0][0]);
	glVertex3fv(&Vertices[4][0]);
	glVertex3fv(&Vertices[6][0]);

	glVertex3fv(&Vertices[0][0]);
	glVertex3fv(&Vertices[6][0]);
	glVertex3fv(&Vertices[7][0]);

	glVertex3fv(&Vertices[0][0]);
	glVertex3fv(&Vertices[7][0]);
	glVertex3fv(&Vertices[1][0]);

	glEnd();
}
