// PowerUp.h
#pragma once
#include "raylib.h"

enum PowerUpType {
    NONE,             // No power-up
    PADDLE_SIZE,      // Increases paddle size
    FAST_BALL,        // Temporarily increases ball speed
    PENETRATING_BALL  // Makes the ball penetrate through bricks
};

struct PowerUp {
    PowerUpType type;
    Rectangle rect;
    bool active;
    float duration;
    float timer;

    PowerUp() {
        type = NONE;
        rect = {0, 0, 20, 20};
        active = false;
        duration = 0;
        timer = 0;
    }
};

// Draws the power-up on the screen if active
void DrawPowerUp(const PowerUp& powerUp);