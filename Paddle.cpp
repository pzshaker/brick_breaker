#include "Paddle.h"

Paddle::Paddle() {
    defaultWidth = 100;
    rect = {350, 550, defaultWidth, 20};
    sizeIncreased = false;
    timer = 0;
}

// Updates paddle position to follow the mouse
void Paddle::Update() {
    float mouseX = GetMouseX();
    rect.x = mouseX - rect.width / 2;

    // Ensure paddle stays within screen bounds
    if (rect.x < 0) rect.x = 0;
    if (rect.x + rect.width > GetScreenWidth()) rect.x = GetScreenWidth() - rect.width;
}

void Paddle::Draw() {
    DrawRectangleRec(rect, BLUE);
}

// Applies a size-increase power-up for the specified duration
void Paddle::ApplyPowerUp(float newWidth, float duration) {
    rect.width = newWidth;
    timer = duration;
    sizeIncreased = true;
}

// Updates the power-up timer and reverts paddle size if expired
void Paddle::UpdatePowerUp(float deltaTime) {
    if (sizeIncreased) {
        timer -= deltaTime;
        if (timer <= 0) {
            rect.width = defaultWidth;
            sizeIncreased = false;
        }
    }
}

Rectangle Paddle::GetRect() const {
    return rect;
}