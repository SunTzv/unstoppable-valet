#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"

class Player {
public:
    Vector2 pos;
    bool inCar;
    Texture2D texture;

    Player() {
        pos = { 100, 75 };
        inCar = false;
    }

    void Update() {
        if (!inCar) {
            if (IsKeyDown(KEY_W)) pos.y -= 1.0f;
            if (IsKeyDown(KEY_S)) pos.y += 1.0f;
            if (IsKeyDown(KEY_A)) pos.x -= 1.0f;
            if (IsKeyDown(KEY_D)) pos.x += 1.0f;
        }
    }

    void Draw() {
        if (!inCar) {
            if (texture.id != 0) {
                DrawTexture(texture, (int)pos.x - 4, (int)pos.y - 4, WHITE);
            } else {
                DrawRectangle((int)pos.x - 4, (int)pos.y - 4, 8, 8, YELLOW);
            }
        }
    }
};

#endif