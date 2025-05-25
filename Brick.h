#pragma once
#include "raylib.h"
#include "PowerUp.h"
#include "vector"

const int MAX_BRICKS = 20;

enum BrickType {
    STANDARD,
    DURABLE,
    INDESTRUCTIBLE,
    SPECIAL,
    POWERUP
};

struct BrickData {
    Rectangle rect;
    BrickType type;
    int hitPoints;
};

class Brick {
private:
    BrickData bricks[MAX_BRICKS];
    int count;

public:
    Brick();                      // Constructor to initialize bricks with positions and types
    void Draw();                  // Draws all active bricks on the screen
    BrickData* GetBricks();       // Returns the array of brick data
    int GetCount() const;         // Returns the total number of bricks
    PowerUp TrySpawnPowerUp(Vector2 position, BrickType brickType); // Modified to consider brick type
    void ExplodeNearbyBricks(int brickIndex, std::vector<PowerUp>& outPowerUps); // Destroys adjacent bricks when a Special Brick is broken
};