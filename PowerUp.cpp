#include "PowerUp.h"

// Draws the power-up on the screen with a color based on its type
void DrawPowerUp(const PowerUp& powerUp) {
	if (!powerUp.active) return;

	Color color = BLACK;
	switch (powerUp.type) {
	case PADDLE_SIZE: color = GREEN; break;
	case FAST_BALL: color = RED; break;
	case PENETRATING_BALL: color = PURPLE; break;
	default: return;
	}

	DrawRectangleRec(powerUp.rect, color);
}