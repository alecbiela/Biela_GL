#pragma once
#include "GamePiece.h"
class Camera :
	public GamePiece
{
public:
	Camera();
	~Camera();
	void Update(double deltaTime);
	glm::mat3 rotMat;
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
	glm::mat4 lookAtMat;
	glm::mat4 perspectiveMat;
	
	float zoom;
	int width;
	int height;

	float fovy;
	float aspect;
	float zNear;
	float zFar;
};

