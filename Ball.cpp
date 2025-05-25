#include "Ball.h"
#include "Storage.h"
#include "PowerUp.h"

// Initializes ball at a default position, attached to paddle
Ball::Ball() {
	position = {400.0f, 500.0f};
	speed = {0, 0};
	originalSpeed = {0, 0};
	isAttached = true;
	penetrating = false;
	isFast = false;
	timer = 0;
	fastTimer = 0;

	comboCount = 0;
	comboTimer = 0.0f;
}

void Ball::Update(Rectangle paddle, BrickData bricks[], int brickCount,
	int& score, int& lives, bool& gameOver,
	Brick& brickManager,
	std::vector<PowerUp>& outPowerUps) {
	float dt = GetFrameTime();

	if (comboTimer > 0.0f) {
		comboTimer -= dt;
		if (comboTimer <= 0.0f) {
			comboCount = 0;
		}
	}

	if (isAttached) {
		position.x = paddle.x + paddle.width / 2;
		position.y = paddle.y - 10;
		return;
	}

	// Move ball:
	position.x += speed.x * 300 * dt;
	position.y += speed.y * 300 * dt;

	// Wall collisions
	if (position.x <= 0 || position.x >= GetScreenWidth()) speed.x *= -1;
	if (position.y <= 0) speed.y *= -1;
	if (position.y > GetScreenHeight()) {
		lives--;
		if (lives <= 0) gameOver = true;
		Reset(paddle);
		return;
	}

	// Paddle collision
	if (CheckCollisionCircleRec(position, 10, paddle)) {
		speed.y = -speed.y;
		float offset = (position.x - paddle.x) / paddle.width - 0.5f;
		speed.x = offset * 2.0f;
	}

	// Brick collisions
	for (int i = 0; i < brickCount; i++) {
		if (bricks[i].hitPoints <= 0) continue;

		if (CheckCollisionCircleRec(position, 10, bricks[i].rect)) {
			// Hit a live brick
			if (bricks[i].type != INDESTRUCTIBLE) {
				bricks[i].hitPoints--;
				score += 100;

				// --- COMBO LOGIC ---
				if (bricks[i].hitPoints == 0) {
					// continue or reset combo
					if (comboTimer > 0.0f) comboCount++;
					else comboCount = 1;
					comboTimer = COMBO_WINDOW;

					const int BONUS_PER_CHAIN = 50;
					const int pointsGained = BONUS_PER_CHAIN * (comboCount - 1);
					score += pointsGained;

					if (comboCount >= 2) {
						printf("Combo x%d! Gained +%d bonus points\n",
							comboCount, pointsGained);
					}
				}

				// If a brick get destroyed, spawn its pickup
				if (bricks[i].hitPoints == 0) {
					Vector2 spawnPos = {
						bricks[i].rect.x + bricks[i].rect.width / 2,
						bricks[i].rect.y + bricks[i].rect.height / 2
					};
					PowerUp pu = brickManager.TrySpawnPowerUp(spawnPos, bricks[i].type);
					if (pu.active) outPowerUps.push_back(pu);

					// If it's a SPECIAL brick, explode neighbors and spawn their pickups:
					if (bricks[i].type == SPECIAL) {
						brickManager.ExplodeNearbyBricks(i, outPowerUps);
					}
				}

				// Bounce unless penetrating
				if (!penetrating) speed.y = -speed.y;
			}
			else {
				// Indestructible always bounces
				// Calculate overlap to determine collision side
				float overlapLeft = position.x + 10 - bricks[i].rect.x;
				float overlapRight = (bricks[i].rect.x + bricks[i].rect.width) - (position.x - 10);
				float overlapTop = position.y + 10 - bricks[i].rect.y;
				float overlapBottom = (bricks[i].rect.y + bricks[i].rect.height) - (position.y - 10);

				// Find the minimum overlap to determine the collision side
				float minOverlap = overlapLeft;
				bool horizontal = true;

				if (overlapRight < minOverlap) minOverlap = overlapRight;
				if (overlapTop < minOverlap) {
					minOverlap = overlapTop;
					horizontal = false;
				}
				if (overlapBottom < minOverlap) {
					minOverlap = overlapBottom;
					horizontal = false;
				}

				// Bounce based on collision side
				if (horizontal) {
					speed.x *= -1;
				}
				else {
					speed.y *= -1;
				}
			}

			// If not penetrating, only break on first collision
			if (!penetrating) break;
		}
	}

	// Update any active fast/penetration timers:
	UpdatePowerUp(dt);
}

// Draws the ball as a circle
void Ball::Draw() {
	Color color = BLACK;
	if (penetrating) color = PURPLE;
	else if (isFast) color = RED;
	DrawCircleV(position, 10, color);
}

// Resets the ball to its initial state on the paddle
void Ball::Reset(Rectangle paddle) {
	position = {paddle.x + paddle.width / 2, paddle.y - 10};
	speed = {0, 0};
	originalSpeed = {0, 0};
	isAttached = true;
	penetrating = false;
	isFast = false;
	timer = 0;
	fastTimer = 0;
}

// Launches the ball upward from the paddle
void Ball::Launch() {
	if (isAttached) {
		speed = {0, -1};
		originalSpeed = speed;
		isAttached = false;
	}
}

// Applies a power-up for the specified duration
void Ball::ApplyPowerUp(PowerUpType type, float duration) {
	if (type == PENETRATING_BALL) {
		penetrating = true;
		timer = duration;
	}
	else if (type == FAST_BALL) {
		if (!isFast) {
			originalSpeed = {(float) fabs(speed.x), (float) fabs(speed.y)};
			speed.x *= 1.5f;
			speed.y *= 1.5f;
			isFast = true;
			fastTimer = duration;
		}
	}
}

// Updates the penetrating and fast power-up timers and reverts if expired
void Ball::UpdatePowerUp(float deltaTime) {
	if (penetrating) {
		timer -= deltaTime;
		if (timer <= 0) penetrating = false;
	}
	if (isFast) {
		fastTimer -= deltaTime;
		if (fastTimer <= 0) {
			float signX = (speed.x >= 0) ? 1.0f : -1.0f;
			float signY = (speed.y >= 0) ? 1.0f : -1.0f;
			speed = {signX * originalSpeed.x, signY * originalSpeed.y};
			isFast = false;
		}
	}
}

// Sets the ball's speed to a new value
void Ball::SetSpeed(Vector2 newSpeed) {
	speed = newSpeed;
	if (!isFast) originalSpeed = newSpeed;
}

bool Ball::IsAttached()   const { return isAttached; }
bool Ball::IsPenetrating()const { return penetrating; }
bool Ball::IsFast()       const { return isFast; }
Vector2 Ball::GetPosition() const { return position; }
Vector2 Ball::GetSpeed()    const { return speed; }