#pragma once
#include "GamePiece.h"

class Player :
	public GamePiece
{
public:
	Player();
	~Player();
	int health;
	void Update(double deltaTime);
	void Move(float speed);
};

