#include "Brick.h"
#include "vector"

// Initializes bricks in a centered grid
Brick::Brick() {
	count = MAX_BRICKS;
	const int columns = 4;
	float brickWidth = 180;
	float brickHeight = 30;
	float gap = 10;
	float startX = 25;
	float startY = 60;

	int row = 0;
	int col = 0;

	for (int i = 0; i < count; i++) {
		float x = startX + col * (brickWidth + gap);
		float y = startY + row * (brickHeight + gap);
		bricks[i].rect = {x, y, brickWidth, brickHeight};

		// Assign brick types
		if (i == 10 || i == 12) {
			bricks[i].type = INDESTRUCTIBLE;
			bricks[i].hitPoints = 999;
		}
		else if (i == 7 || i == 12) {
			bricks[i].type = SPECIAL;
			bricks[i].hitPoints = 1;
		}
		else if (i == 3 || i == 15) {
			bricks[i].type = POWERUP;
			bricks[i].hitPoints = 1;
		}
		else if (i % 3 - 2 == 0) {
			bricks[i].type = DURABLE;
			bricks[i].hitPoints = 2;
		}
		else {
			bricks[i].type = STANDARD;
			bricks[i].hitPoints = 1;
		}

		col++;
		if (col >= columns) {
			col = 0;
			row++;
		}
	}
}

// Draws all active bricks with colors based on their type and hit points
void Brick::Draw() {
	for (int i = 0; i < count; i++) {
		if (bricks[i].hitPoints > 0) {
			Color color = RED;
			if (bricks[i].type == DURABLE) {
				color = (bricks[i].hitPoints == 2) ? ORANGE : RED;
			}
			else if (bricks[i].type == INDESTRUCTIBLE) {
				color = GRAY;
			}
			else if (bricks[i].type == SPECIAL) {
				color = SKYBLUE;
			}
			else if (bricks[i].type == POWERUP) {
				color = YELLOW;
			}
			DrawRectangleRec(bricks[i].rect, color);
		}
	}
}

BrickData* Brick::GetBricks() {
	return bricks;
}

int Brick::GetCount() const {
	return count;
}

// Attempts to spawn a power-up at the given position, guaranteed for POWERUP bricks
PowerUp Brick::TrySpawnPowerUp(Vector2 position, BrickType brickType) {
	PowerUp powerUp;
	bool spawnPowerUp = false;

	if (brickType == POWERUP) {
		spawnPowerUp = true;
	}
	else {
		int chance = GetRandomValue(1, 100);
		if (chance <= 20) {
			spawnPowerUp = true;
		}
	}

	if (spawnPowerUp) {
		powerUp.active = true;
		powerUp.rect = {position.x - 10, position.y, 20, 20};
		int type = GetRandomValue(1, 3);
		switch (type) {
		case 1: powerUp.type = PADDLE_SIZE; powerUp.duration = 5.0f; break;
		case 2: powerUp.type = FAST_BALL; powerUp.duration = 5.0f; break;
		case 3: powerUp.type = PENETRATING_BALL; powerUp.duration = 5.0f; break;
		}
	}
	return powerUp;
}

void Brick::ExplodeNearbyBricks(int brickIndex, std::vector<PowerUp>& outPowerUps) {
	const int columns = 4;
	const int offsets[] = {-1, 1, -columns, columns};
	int row = brickIndex / columns;

	for (int offset : offsets) {
		int adjIndex = brickIndex + offset;
		if (adjIndex < 0 || adjIndex >= count) continue;
		if ((offset == -1 || offset == 1) && (adjIndex / columns != row)) continue;
		if (bricks[adjIndex].type == INDESTRUCTIBLE) continue;

		Vector2 pos = {
			bricks[adjIndex].rect.x + bricks[adjIndex].rect.width / 2,
			bricks[adjIndex].rect.y + bricks[adjIndex].rect.height / 2
		};

		PowerUp powerup = TrySpawnPowerUp(pos, bricks[adjIndex].type);
		if (powerup.active) outPowerUps.push_back(powerup);
		bricks[adjIndex].hitPoints = 0;
	}
}
