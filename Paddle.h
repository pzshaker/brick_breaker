// Paddle.h
#pragma once
#include "raylib.h"

class Paddle {
private:
	Rectangle rect;
	float defaultWidth;
	bool sizeIncreased;
	float timer;

public:
	Paddle();
	void Update();
	void Draw();
	void ApplyPowerUp(float newWidth, float duration);
	void UpdatePowerUp(float deltaTime);
	Rectangle GetRect() const;
};