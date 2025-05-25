// Main.cpp
#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include "PowerUp.h"
#include "Storage.h"
#include <vector>

enum GameState {
	PLAYING,       // Game is in progress
	GAME_OVER,     // Player has lost all lives
	LEVEL_COMPLETE // All breakable bricks are destroyed
};

// Points awarded for catching each power-up
static int PointsForPowerUp(PowerUpType type) {
	switch (type) {
	case PADDLE_SIZE:      return 20;
	case FAST_BALL:        return 50;
	case PENETRATING_BALL: return 70;
	default:               return 0;
	}
}

// Update all game objects, collect any spawned power-ups into powerUps[]
void UpdateGameObjects(Paddle& paddle, Ball& ball, Brick& brick,
	std::vector<PowerUp>& powerUps,
	int& score, int& lives, GameState& state,
	int screenHeight) {
	float dt = GetFrameTime();

	// Paddle movement & its power-up timer
	paddle.Update();
	paddle.UpdatePowerUp(dt);

	bool gameOver = false;
	// Collect new drops from ball collisions / explosions
	std::vector<PowerUp> newDrops;
	ball.Update(paddle.GetRect(), brick.GetBricks(), brick.GetCount(),
		score, lives, gameOver, brick, newDrops);
	// Append them to our master list
	powerUps.insert(powerUps.end(), newDrops.begin(), newDrops.end());

	// Advance & collect each active power-up
	for (auto it = powerUps.begin(); it != powerUps.end(); ) {
		it->rect.y += 200 * dt;

		// off-screen?
		if (it->rect.y > screenHeight) {
			it = powerUps.erase(it);
		}
		// caught by paddle?
		else if (CheckCollisionRecs(it->rect, paddle.GetRect())) {
			// 1) award points
			score += PointsForPowerUp(it->type);

			// 2) apply the power-up effect
			switch (it->type) {
			case PADDLE_SIZE:
				paddle.ApplyPowerUp(150, it->duration);
				break;
			case FAST_BALL:
				ball.ApplyPowerUp(FAST_BALL, it->duration);
				break;
			case PENETRATING_BALL:
				ball.ApplyPowerUp(PENETRATING_BALL, it->duration);
				break;
			default:
				break;
			}

			// 3) remove it from the world
			it = powerUps.erase(it);
		}
		else {
			++it;
		}
	}

	if (gameOver) state = GAME_OVER;
}

// Check if all breakable bricks are gone
void CheckWinCondition(Brick& brick, GameState& state) {
	int remaining = 0;
	for (int i = 0; i < brick.GetCount(); i++) {
		const BrickData& b = brick.GetBricks()[i];
		if (b.type != INDESTRUCTIBLE && b.hitPoints > 0) remaining++;
	}
	if (remaining == 0) state = LEVEL_COMPLETE;
}

// Update high score if beaten
void UpdateHighScore(int& highScore, int score) {
	if (score > highScore) highScore = score;
}

// Handle launch & restart (save on restart, clear power-ups)
void HandleInput(Ball& ball, GameState& state, Paddle& paddle, Brick& brick,
	int& score, int& lives, int& highScore,
	std::vector<PowerUp>& powerUps) {
	if (state == PLAYING && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		ball.Launch();
	}

	if ((state == GAME_OVER || state == LEVEL_COMPLETE) && IsKeyPressed(KEY_R)) {
		SaveStorageValue(0, highScore); // save once on restart

		// reset everything
		ball.Reset(paddle.GetRect());
		paddle = Paddle();
		brick = Brick();
		score = 0;
		lives = 3;
		state = PLAYING;
		powerUps.clear();              // remove any leftover drops
	}
}

// Draw all game elements including every active power-up
void DrawGame(Paddle& paddle, Brick& brick, Ball& ball,
	const std::vector<PowerUp>& powerUps,
	int score, int highScore, int lives,
	GameState state, int screenWidth) {
	BeginDrawing();
	ClearBackground(RAYWHITE);

	paddle.Draw();
	brick.Draw();
	ball.Draw();
	for (const auto& pu : powerUps) DrawPowerUp(pu);

	DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);
	int w = MeasureText(TextFormat("High Score: %d", highScore), 20);
	DrawText(TextFormat("High Score: %d", highScore),
		screenWidth - w - 10, 10, 20, BLACK);
	DrawText(TextFormat("Lives: %d", lives), 10, 30, 20, BLACK);

	if (state == GAME_OVER) {
		DrawText("Game Over! Press R to Restart", 250, 300, 20, RED);
	}
	else if (state == LEVEL_COMPLETE) {
		DrawText("Level Complete! Press R to Restart", 230, 300, 20, GREEN);
	}

	EndDrawing();
}

int main() {
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Raylib Brick Breaker");
	SetTargetFPS(60);

	Ball ball;
	Paddle paddle;
	Brick brick;
	std::vector<PowerUp> powerUps;      // now a list
	int score = 0;
	int lives = 3;
	int highScore = LoadStorageValue(0);
	GameState state = PLAYING;

	while (!WindowShouldClose()) {
		if (state == PLAYING) {
			UpdateGameObjects(paddle, ball, brick, powerUps,
				score, lives, state, screenHeight);
			CheckWinCondition(brick, state);
			UpdateHighScore(highScore, score);
		}

		HandleInput(ball, state, paddle, brick,
			score, lives, highScore, powerUps);
		DrawGame(paddle, brick, ball, powerUps,
			score, highScore, lives, state, screenWidth);
	}

	// Save on exit
	SaveStorageValue(0, highScore);

	CloseWindow();
	return 0;
}
