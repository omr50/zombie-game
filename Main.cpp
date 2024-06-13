#include <iostream>
#include "raylib.h"
#include "Motion.h"
#include "ScalingComponent.h"
#include "AnimatableSprite.h"
#include "SoldierComponent.h"
#include "AnimatableEnemy.h"
#include "ZombieManager.h"
float scaleFactor;
void KeyboardLogic(Soldier* sprite, int* offset);

int main() {
    // Initialization

    int screenWidth = 800;
    int screenHeight = 600;
    SetTargetFPS(15);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Windowed Mode");
    Rectangle startButton = { screenWidth / 2 - 100, screenHeight / 2 - 20, 200, 40 };
    bool startButtonHover = false;
    bool gameStarted = false;
    bool finished = false;
    int highScore = 0;
    
    Texture2D background = LoadTexture("C:/Users/omrab/source/repos/GameJam_raylib/x64/Debug/Sprites/Soldier/apoc.png");
    Motion soldierDimensions = Motion({ 500, 450 }, 313, 220, 1, 180, 2.0, true);
    ScalingComponent soldierScale = ScalingComponent(0.35, &scaleFactor);

    Soldier soldier = Soldier(soldierDimensions, soldierScale);
    soldier.animationComponent
        .addAnimationState(SHOOTING_SHOTGUN, "resources/rifle/shoot/merged.png", 3, 3, 312, 206)
        ->addAnimationState(IDLE_SHOTGUN, "resources/rifle/shoot/merged.png", 1, 1, 312, 206)
        ->addAnimationState(MOVING_SHOTGUN, "resources/rifle/walk/merged.png", 20, 20, 313, 206)

        ->addAnimationState(IDLE_PISTOL, "resources/handgun/shoot/merged.png", 1, 1, 255, 215)
        ->addAnimationState(SHOOTING_PISTOL, "resources/handgun/shoot/merged.png", 3, 3, 255, 215)
        ->addAnimationState(MOVING_PISTOL, "resources/handgun/walk/merged.png", 20, 20, 258, 220)

        ->addAnimationState(IDLE_RIFLE, "resources/shotgun/shoot/merged.png", 1, 1, 312, 206)
        ->addAnimationState(SHOOTING_RIFLE, "resources/shotgun/shoot/merged.png", 3, 3, 312, 206)
        ->addAnimationState(MOVING_RIFLE, "resources/shotgun/walk/merged.png", 20, 20, 313, 206)

        ->addAnimationState(IDLE_FEET, "resources/feet/walk/merged.png", 1, 1, 172, 124)
        ->addAnimationState(MOVING_FEET, "resources/feet/walk/merged.png", 20, 20, 172, 124);

    ZombieManager zombieManager = ZombieManager(&soldier);
    int numZombies = 20;
    zombieManager.createNZombies(numZombies);
    
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        if (soldier.healthBar <= 0) {
            gameStarted = false;
            finished = true;
            highScore = std::max(zombieManager.waveNumber + 1, highScore);

        }
        if (!gameStarted) {
            Vector2 mousePoint = GetMousePosition();
            startButtonHover = CheckCollisionPointRec(mousePoint, startButton);

            if (startButtonHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                gameStarted = true;
                soldier = Soldier(soldierDimensions, soldierScale);
                zombieManager = ZombieManager(&soldier);
                numZombies = 20;
                zombieManager.createNZombies(20);
            }

            // Draw home page
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Only Survivor", 190, 200, 20, GRAY);
            if (finished) {
                DrawText(("Your highscore is: " + std::to_string(highScore)).c_str(), 190, 250, 17, YELLOW);
                DrawText("Play Again?", 190, 280, 17, WHITE);
            }

            // Draw start button
            DrawRectangleRec(startButton, startButtonHover ? SKYBLUE : LIGHTGRAY);
            DrawText("Start Game", startButton.x + 60, startButton.y + 10, 20, startButtonHover ? DARKBLUE : DARKGRAY);
            EndDrawing();
        }
        else {
            KeyboardLogic(&soldier, &soldier.gun);
            soldier.rotate();
            soldier.update();
            
            ClearBackground(RAYWHITE);
            int currentScreenWidth = GetScreenWidth();
            int currentScreenHeight = GetScreenHeight();
            soldier.camera.offset = { currentScreenWidth / 2.0f, currentScreenHeight / 2.0f };
            // Clamp horizontally
            float halfScreenWidth = currentScreenWidth / 2.0f;
            Vector2 clampedPosition = soldier.scales.getScaledPosition(soldier.motions.position);
            clampedPosition.x = fmax(clampedPosition.x, halfScreenWidth);
            clampedPosition.x = fmin(clampedPosition.x, 4000 - halfScreenWidth);

            // Clamp vertically
            float halfScreenHeight = currentScreenHeight / 2.0f;
            clampedPosition.y = fmax(clampedPosition.y, halfScreenHeight);
            clampedPosition.y = fmin(clampedPosition.y, 3181 - halfScreenHeight);

            // Update camera target with clamped position
            soldier.camera.target = clampedPosition;
            //soldier.camera.target = soldier.scales.getScaledPosition(soldier.motions.position);
            scaleFactor = fmin(currentScreenWidth / (float)screenWidth, currentScreenHeight / (float)screenHeight);

            BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(soldier.camera);
            DrawTexture(background, 0, 0, WHITE);
            zombieManager.findClosestZombiesToDamage(soldier.bullets);
            zombieManager.findClosestZombiesToTowerDamage();
            zombieManager.update();
            soldier.draw();
            zombieManager.draw();
            if (zombieManager.zombies.size() == 0) {
                numZombies += 10;
                zombieManager.createNZombies(numZombies);
            }
            EndMode2D();

            
            DrawRectangle(0, 0, 170, 100, { 0, 0, 0, 120 });
            DrawText("HP: ", 20, 10, 30, RED);
            DrawText(+std::to_string((int)soldier.healthBar).c_str(), 70, 10, 35, WHITE);
            DrawText(("Zombies: " + std::to_string(zombieManager.zombies.size())).c_str(), 20, 40, 25, GREEN);
            DrawText(("Wave: " + std::to_string(zombieManager.waveNumber)).c_str(), 20, 70, 20, YELLOW);

            DrawText("Pistol: inf", 0, GetScreenHeight() - 30, 20, WHITE);
            DrawText(("Rifle: " + std::to_string(soldier.rifleAmmo)).c_str(), 120, GetScreenHeight() - 30, 20, WHITE);
            DrawText(("Sniper: " + std::to_string(soldier.sniperAmmo)).c_str(), 240, GetScreenHeight() - 30, 20, WHITE);
            DrawText(("Turrets: " + std::to_string(soldier.numTurrets)).c_str(), 360, GetScreenHeight() - 30, 20, WHITE);

            EndDrawing();
        }

    }
        CloseWindow();
    return 0;
}

// move to class later
// keyboard logic handler, modifies internal
// state necessary for motion (mode, position
// and direction.) 
void KeyboardLogic(Soldier* sprite, int* itemOffset = NULL) {

    bool movementKeyPressed = false;
    if (IsKeyDown(KEY_ONE)) {
        *itemOffset = 1;
    }

    if (IsKeyDown(KEY_TWO)) {
        *itemOffset = 2;
    }
    if (IsKeyDown(KEY_THREE)) {
        *itemOffset = 3;
    }
    if (IsKeyDown(KEY_A)) {
        movementKeyPressed = true;
        sprite->motions.moveLeft(sprite->mode);
    }
    if (IsKeyDown(KEY_D)) {
        movementKeyPressed = true;
        sprite->motions.moveRight(sprite->mode);
    }

    if (IsKeyDown(KEY_W)) {
        movementKeyPressed = true;
        sprite->motions.moveUp(sprite->mode);
    }

    if (IsKeyDown(KEY_S)) {
        movementKeyPressed = true;
        sprite->motions.moveDown(sprite->mode);
    }

    if (IsMouseButtonDown(0)) {
        movementKeyPressed = true;
        sprite->mode = SHOOTING;
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        sprite->fireProjectile();
    }
    if (!movementKeyPressed) {
        sprite->mode = IDLE;
    }
    
    if (IsKeyDown(KEY_SPACE)) {
        sprite->addTurret();
    }

}