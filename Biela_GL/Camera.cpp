#include "stdafx.h"
#include "Camera.h"
#include "Engine.h"

using namespace glm;

Camera::Camera()
{
	transform.loc = { 0, 0, 2 };
	transform.rotation = { 0, 0, 0 };
	transform.scale = { 1, 1, 1 };
	rigidbody.force = { 0,0,0 };
	rigidbody.mass = 0;
	rigidbody.position = { 0,0,0 };
	rigidbody.velocity = { 0,0,0 };

}


Camera::~Camera()
{
}

void Camera::Update(double deltaTime)
{
	//define perspective variables
	zoom = 1.f;
	fovy = 3.14159f * .4f / zoom;
	width = 800;
	height = 800;
	aspect = (float)width / (float)height;
	zNear = .01f;
	zFar = 1000.f;
	
	vec3 playerPos = Engine::Instance->player.transform.loc;

	//define lookAt variables
	rotMat = (mat3)yawPitchRoll(transform.rotation.y, transform.rotation.x, transform.rotation.z);
	eye = transform.loc;
	center = playerPos;
	up = rotMat * vec3(0, 1, 0);

	//define camera
	lookAtMat = lookAt(eye, center, up);
	perspectiveMat = perspective(fovy, aspect, zNear, zFar);

	worldMatrix = perspectiveMat * lookAtMat;

	//send matrix to vertex shader
	glUniformMatrix4fv(10, 1, GL_FALSE, &worldMatrix[0][0]);
}
