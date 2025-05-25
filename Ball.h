#pragma once
#include "raylib.h"
#include "Brick.h"
#include "vector"

class Ball {
private:
    Vector2 position;
    Vector2 speed;
    Vector2 originalSpeed;
    bool isAttached;
    bool penetrating;
    bool isFast;
    float timer;
    float fastTimer;
    int   comboCount;
    float comboTimer;
	const float COMBO_WINDOW = 1.0f;
public:
    Ball();               // Constructor to initialize ball properties
    void Update(Rectangle paddle, BrickData bricks[], int brickCount, int& score, int& lives, bool& gameOver, Brick& brickManager, std::vector<PowerUp>& outPowerUps);
    void Draw();
    void Reset(Rectangle paddle);
    void Launch();
    void ApplyPowerUp(PowerUpType type, float duration);
    void UpdatePowerUp(float deltaTime);
    bool IsAttached() const;
    bool IsPenetrating() const;
    bool IsFast() const;
    Vector2 GetPosition() const;
    Vector2 GetSpeed() const;
    void SetSpeed(Vector2 newSpeed);
};