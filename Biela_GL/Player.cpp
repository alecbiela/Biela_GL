#include "stdafx.h"
#include "Player.h"

using namespace glm;

Player::Player()
{
	radius = 0.09f;
	transform.loc = { 0,0,0 };
	transform.rotation = { 0,0,0 };
	transform.scale = { .1, .1, .1 };
	rigidbody.force = { 0,0,0 };
	rigidbody.mass = 0;
	rigidbody.position = { 0,0,0 };
	rigidbody.velocity = { 0,0,0 };
	rigidbody.forward = { 0,0,0 };
}


Player::~Player()
{
}

//update is called once per frame
void Player::Update(double deltaTime)
{

	//calculate forward vector
	vec3 desired = vec3(glm::sin(transform.rotation.y + 3.14159f / 2), 0, glm::cos(transform.rotation.y + 3.14159f / 2));
	rigidbody.forward = glm::normalize(desired);

	//define world matrix
	worldMatrix = translate(transform.loc)
		* yawPitchRoll(transform.rotation.y,
			transform.rotation.x,
			transform.rotation.z)
		* scale(transform.scale);

	//send matrix to vertex shader
	glUniformMatrix4fv(6, 1, GL_FALSE, &worldMatrix[0][0]);
}

//moves the player (called from other classes)
void Player::Move(float speed)
{
	transform.loc += rigidbody.forward * speed;
}
