#ifndef CAR_H
#define CAR_H

#include "common.h"
#include "raymath.h"

class Car {
public:
    Vector2 pos;
    float rotation; 
    float speed;
    float width = 64; 
    float height = 128;
    bool isOccupied;
    Texture2D texture;

    Car(Vector2 startPos) {
        pos = startPos;
        rotation = -90.0f; // Start facing Up
        speed = 0;
        isOccupied = false;
        texture.id = 0;
    }

    void Update() {
        if (!isOccupied) {
            speed *= 0.95f; 
        } else {
            if (IsKeyDown(KEY_W)) speed += 0.15f;
            if (IsKeyDown(KEY_S)) speed -= 0.10f;
            if (fabs(speed) > 0.1f) {
                if (IsKeyDown(KEY_A)) rotation -= 3.0f;
                if (IsKeyDown(KEY_D)) rotation += 3.0f;
            }
            speed *= 0.98f;
        }

        pos.x += cosf(rotation * DEG2RAD) * speed;
        pos.y += sinf(rotation * DEG2RAD) * speed;
    }

    void Draw() {
        if (texture.id != 0) {
            Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
            Rectangle dest = { pos.x, pos.y, width, height };
            Vector2 origin = { width / 2, height / 2 };

            // We add 90 degrees because the sprite itself is vertical
            DrawTexturePro(texture, source, dest, origin, rotation + 90.0f, WHITE);
        }
    }
};

#endif