#pragma once
#include "stdafx.h"

struct Transform
{
	glm::vec3 loc;
	glm::vec3 rotation;
	glm::vec3 scale;
};

struct RigidBody
{
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;
	float mass;
};

class GamePiece
{
public:
	GamePiece();
	GamePiece(std::string txPath);
	virtual ~GamePiece();
	virtual void Update(double deltaTime) = 0;
	Transform transform;
	RigidBody rigidbody;
	std::string texturePath;
	glm::mat4 worldMatrix;
	bool IsColliding(const GamePiece &other);
	float radius;
};

