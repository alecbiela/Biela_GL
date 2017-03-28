#include "stdafx.h"
#include "GamePiece.h"

GLuint textureIds[4];

using namespace glm;

GamePiece::GamePiece()
{
}

GamePiece::GamePiece(std::string txPath)
{
	texturePath = txPath;
}

bool GamePiece::IsColliding(const GamePiece &other)
{
	float distSq = distance(this->transform.loc, other.transform.loc);
	distSq = distSq * distSq;
	float distToCollideSq = this->radius + other.radius;
	distToCollideSq = distToCollideSq * distToCollideSq;

	if (distSq < distToCollideSq) return true;
	else return false;
}

GamePiece::~GamePiece()
{
}
