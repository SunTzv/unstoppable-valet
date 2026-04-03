#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"

enum Direction { DIR_S = 0, DIR_W = 1, DIR_A = 2, DIR_D = 3 };

class Player {
public:
    Vector2 pos;
    bool inCar;
    Texture2D spriteSheet;
    Direction facing;
    int currentFrame;
    float frameTimer;
    float frameSpeed;
    bool isMoving;

    const int RENDER_SIZE = 48;

    Player() {
        pos = { 400, 300 };
        inCar = false;
        facing = DIR_S;
        currentFrame = 0;
        frameTimer = 0.0f;
        frameSpeed = 0.12f;
        isMoving = false;
    }

    void Update() {
        if (!inCar) {
            isMoving = false;
            float speed = 3.8f;

            if (IsKeyDown(KEY_W)) { pos.y -= speed; facing = DIR_W; isMoving = true; }
            if (IsKeyDown(KEY_S)) { pos.y += speed; facing = DIR_S; isMoving = true; }
            if (IsKeyDown(KEY_A)) { pos.x -= speed; facing = DIR_A; isMoving = true; }
            if (IsKeyDown(KEY_D)) { pos.x += speed; facing = DIR_D; isMoving = true; }

            // 32px Border Constraint
            float margin = 32.0f + 15.0f;
            pos.x = Clamp(pos.x, margin, SCREEN_W - margin);
            pos.y = Clamp(pos.y, margin, SCREEN_H - margin);

            if (isMoving) {
                frameTimer += GetFrameTime();
                if (frameTimer > frameSpeed) {
                    frameTimer = 0.0f;
                    currentFrame = (currentFrame + 1) % 4;
                }
            } else { currentFrame = 0; }
        }
    }

    void Draw() {
        if (!inCar && spriteSheet.id != 0) {
            int sw = spriteSheet.width / 4;
            int sh = spriteSheet.height / 4;
            Rectangle source = { (float)(currentFrame * sw), (float)(facing * sh), (float)sw, (float)sh };
            Rectangle dest = { pos.x, pos.y, (float)RENDER_SIZE, (float)RENDER_SIZE };
            Vector2 origin = { (float)RENDER_SIZE / 2, (float)RENDER_SIZE / 2 };
            DrawTexturePro(spriteSheet, source, dest, origin, 0.0f, WHITE);
        }
    }
};

#endif