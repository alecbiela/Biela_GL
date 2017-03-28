#pragma once
#include "stdafx.h"
#include "Engine.h"

using namespace glm;


extern GLuint textureIds[3];
GLuint currentTexture = 0;
GLfloat rotationValue = 0;
GLuint numPlanets = 2;
GLuint asteroidsPerPlanet = 150000;
mat4* planetMatrices;
mat4* asteroidMatrices;
Engine* Engine::Instance;

Engine::Engine()
{
	Engine::Instance = this;
}

//initialize GLFW, Window, and GLEW
bool Engine::Init()
{
	textureNum = 0;

	//initialize Game Pieces
	player = Player();
	worldCamera = Camera();
	
	//initialize models
	model1 = new Model();
	model2 = new Model();
	model3 = new Model();

	//1.4. Initialize GLFW
	if (glfwInit() == GL_FALSE) return false;

	//1.5. create a windowed mode window
	GLFWwindowPtr =
		glfwCreateWindow(800, 800, "Alec Bielanos openGL Engine",
			NULL, NULL);
	//1.5. make it the currently active window
	if (GLFWwindowPtr != nullptr)
		glfwMakeContextCurrent(GLFWwindowPtr);
	else {
		glfwTerminate();
		return false;
	}

	//set the click function when loading
	glfwSetMouseButtonCallback(GLFWwindowPtr, MouseClick);
	//set the keypress function when loading
	glfwSetKeyCallback(GLFWwindowPtr, KeyCallback);


	//1.6. Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}

	//Generate the World (Populate GameObjects)
	std::cout << "Beginning to generate world..." << std::endl;
	GenerateWorld();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//set up transparency
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

//loads and buffers a model
bool Engine::BufferModels()
{
	if (!(model1->Buffer("models/asteroid.obj"))) return false;
	if (!(model2->Buffer("models/planet.obj"))) return false;
	if (!(model3->Buffer("models/rocketShip.obj"))) return false;
	return true;
}

void Engine::Texture(const char* path)
{
	//loading textures before loop
	textures[0] = FreeImage_ConvertTo32Bits(
		FreeImage_Load(
			FreeImage_GetFileType("textures/shipTexture.png", 0),
			"textures/shipTexture.png"));

	textures[1] = FreeImage_ConvertTo32Bits(
		FreeImage_Load(
			FreeImage_GetFileType("textures/planetTexture.png", 0),
			"textures/planetTexture.png"));

	textures[2] = FreeImage_ConvertTo32Bits(
		FreeImage_Load(
			FreeImage_GetFileType("textures/asteroidTexture.png", 0),
			"textures/asteroidTexture.png"));
	
	for (int i = 0; i < 3; i++)
	{

		int width = FreeImage_GetWidth(textures[i]);
		int height = FreeImage_GetHeight(textures[i]);
		GLubyte* address = FreeImage_GetBits(textures[i]);

		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		glTexImage2D(GL_TEXTURE_2D, 0,
			GL_SRGB_ALPHA,
			width,
			height,
			0, GL_BGRA, GL_UNSIGNED_BYTE,
			(void*)address);

		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		textureIds[i] = texID;

		FreeImage_Unload(textures[i]);
	}
}

//loops while the window is open
bool Engine::GameLoop()
{
	//send matrix information to be instanced
	sm->UseProgram(1);
	model2->InstanceModel(planetMatrices, numPlanets);
	model1->InstanceModel(asteroidMatrices, numPlanets * asteroidsPerPlanet);

	//1.7. Create a Game Loop
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
		//clear the canvas
		glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//update time
		previousTime = currentTime;
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;

		//std::cout << "FPS: " << (1 / deltaTime) << std::endl;
		worldCamera.transform.loc = player.transform.loc + (player.rigidbody.forward * -2.0f);
		worldCamera.transform.loc.y += 1;

		//switch to instancing shader
		sm->UseProgram(1);

		//update the camera
		worldCamera.Update(deltaTime);

		//draw planets
		glBindTexture(GL_TEXTURE_2D, textureIds[1]);
		model2->Render();

		//draw asteroids
		glBindTexture(GL_TEXTURE_2D, textureIds[2]);
		model1->Render();

		//switch to non-instancing shader
		sm->UseProgram(0);

		//update the camera
		worldCamera.Update(deltaTime);

		//draw the player
		player.transform.rotation.y = rotationValue;
		player.Update(deltaTime);
		glBindTexture(GL_TEXTURE_2D, textureIds[0]);
		model3->Render();

		//swap buffers
		glfwSwapBuffers(GLFWwindowPtr);

		//process queued window, mouse & keyboard callback events
		glfwPollEvents();
	}

	//1.8. Before leaving the main, call glfwTerminate();
	glfwTerminate();
	return true;
}

//returns true if the shaders were loaded successfully
bool Engine::UseShaders()
{
	sm = new ShaderManager();
	if (sm->LoadShaders("shaders/vShader.glsl", "shaders/ivShader.glsl", "shaders/fShader.glsl")) return true;
	return false;
}

//clear memory in destructor
Engine::~Engine()
{
	delete sm;
	for (int i = 0; i < 3; i++)
	{
		glDeleteTextures(1, &textureIds[i]);
	}

	Engine::Instance = NULL;

	delete model1;
	delete model2;
	delete model3;
	delete[] planetMatrices;
	delete[] asteroidMatrices;
}

//generates the world based on how many items of each are specified in the top of this class
void Engine::GenerateWorld()
{
	//seed the random
	srand(glfwGetTime());

	//set up collections of matrices
	planetMatrices = new mat4[numPlanets];
	asteroidMatrices = new mat4[asteroidsPerPlanet * numPlanets];
	

	//generates a random planet
	for (GLuint i = 0; i < numPlanets; i++)
	{
		mat4 currentPlanet;

		//translate
		GLfloat x = RandomInRange(-100, 100);
		GLfloat y = RandomInRange(-5, 5);
		GLfloat z = RandomInRange(-100, 100);
		GLfloat offset = 15.f;
		x += (offset * i);
		z -= (offset * i);
		currentPlanet = glm::translate(currentPlanet, vec3(x, y, z));

		//scale
		GLfloat uniformScale = RandomInRange(1, 2);
		currentPlanet = glm::scale(currentPlanet, vec3(uniformScale));

		//add to list
		planetMatrices[i] = currentPlanet;

		//generates asteroids around the planet
		for (GLuint j = 0; j < asteroidsPerPlanet; j++)
		{
			mat4 currentAsteroid;

			//translate
			GLfloat angleFromPlanet = (360.f/asteroidsPerPlanet) * (j+1);
			GLfloat ax = x + RandomInRange(6, 38.5f) * glm::cos(angleFromPlanet);
			GLfloat ay = y + RandomInRange(-0.9, 0.9);
			GLfloat az = z + RandomInRange(6, 38.5f) * glm::sin(angleFromPlanet);
			currentAsteroid = glm::translate(currentAsteroid, vec3(ax, ay, az));

			//scale
			GLfloat asteroidScale = RandomInRange(0.05f, 0.1f);
			currentAsteroid = glm::scale(currentAsteroid, vec3(asteroidScale));

			//rotate
			GLfloat rotation = RandomInRange(0, 360);
			currentAsteroid = glm::rotate(currentAsteroid, rotation, vec3(0.2f, 0.6f, 0.4f));

			//add to list
			asteroidMatrices[j + (asteroidsPerPlanet * i)] = currentAsteroid;
		}
	}

	std::cout << "Generated " << numPlanets << " planets and " << (numPlanets * asteroidsPerPlanet) << " asteroids." << std::endl;
}

//gets a random float between l and u (non-inclusive)
GLfloat Engine::RandomInRange(GLfloat l, GLfloat u)
{
	int randNum = rand() % 1000;
	GLfloat tmp = (randNum / 1000.f) + 0.05;
	tmp *= u;
	tmp += l;

	return tmp;
}

//input handling
void Engine::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_W)
	{
		Engine::Instance->player.Move(0.25f);
	}
	if (key == GLFW_KEY_S)
	{
		Engine::Instance->player.Move(-0.25f);
	}
	if (key == GLFW_KEY_A)
	{
		rotationValue += 0.25f;
	}
	if (key == GLFW_KEY_D)
	{
		rotationValue -= 0.25f;
	}
}

void Engine::MouseClick(GLFWwindow * window, int button, int action, int mods)
{
}



