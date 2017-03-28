#pragma once
#include "stdafx.h"
#include "ShaderManager.h"
#include "GamePiece.h"
#include "Player.h"
#include "Camera.h"
#include "Model.h"


class Engine
{
public:
	Engine();
	bool Init();
	bool BufferModels();
	bool GameLoop();
	bool UseShaders();
	void Texture(const char* path);
	~Engine();
	static Engine* Instance;
	Player player;
private:
	//players and NPCs (and now camera!)

	void GenerateWorld();
	GLfloat RandomInRange(GLfloat l, GLfloat u);

	Camera worldCamera;

	//game time
	float currentTime = 0;
	float previousTime = 0;
	float deltaTime = 0;

	//textures and shaders
	FIBITMAP* textures[3];
	GLuint texID;
	int textureNum;
	ShaderManager* sm;

	//actual OpenGL variables
	GLFWwindow* GLFWwindowPtr;
	Model* model1;
	Model* model2;
	Model* model3;


	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseClick(GLFWwindow* window, int button, int action, int mods);
};



