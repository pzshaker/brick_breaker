# 🎮 Brick Breaker Game (Raylib)

A dynamic brick-breaking arcade game written in C++ using the [Raylib](https://www.raylib.com/) graphics library. This project revives the retro feel of classic brick breaker games with added depth through modern mechanics and a modular design.

## ✨ Features

This project implements a complete 2D brick breaker game with the following capabilities:

- 🎯 **Responsive Gameplay**  
  Control the paddle with your mouse for smooth and intuitive movement. Launch the ball to begin the action and maintain control with accurate rebounds.

- 🧱 **Multiple Brick Types**  
  - **Standard Bricks**: Basic breakable bricks.
  - **Durable Bricks**: Require multiple hits to destroy.
  - **Indestructible Bricks**: Cannot be broken, add strategic challenge.
  - **Special Bricks**: Trigger explosions that destroy nearby bricks.
  - **Power-Up Bricks**: Always drop a power-up upon destruction.

- 💥 **Power-Up System**  
  Collect falling power-ups that grant temporary advantages:
  - `PADDLE_SIZE`: Increases the paddle width.
  - `FAST_BALL`: Temporarily increases ball speed.
  - `PENETRATING_BALL`: Ball passes through bricks without bouncing.

- 🔁 **Combo Scoring System**  
  Earn bonus points by breaking bricks in rapid succession. Each successful combo increases the score multiplier for a limited time.

- 🧠 **Game State Management**  
  The game handles multiple states including:
  - Playing
  - Game Over
  - Level Complete

- 🧷 **High Score Persistence**  
  Player's high score is saved locally between sessions via file storage.

- 🖼️ **Simple Visual Feedback**  
  Each game element is color-coded for clarity and immediate feedback, e.g., different power-ups and brick types are visually distinct.
