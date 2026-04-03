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
    float radius = 45; // Collision radius
    bool isOccupied;
    Texture2D texture;

    Car(Vector2 startPos) {
        pos = startPos;
        rotation = 0.0f; 
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

        // 32px Border Clamping (Except for the Exit area)
        // If not near the exit (100, 600), stay inside borders
        if (!(pos.x > 50 && pos.x < 150 && pos.y > 500)) {
            pos.x = Clamp(pos.x, 32 + width/2, SCREEN_W - 32 - width/2);
            pos.y = Clamp(pos.y, 32 + height/2, SCREEN_H - 32 - height/2);
        }
    }

    void Draw() {
        if (texture.id != 0) {
            Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
            Rectangle dest = { pos.x, pos.y, width, height };
            Vector2 origin = { width / 2, height / 2 };
            DrawTexturePro(texture, source, dest, origin, rotation + 90.0f, WHITE);
        }
    }
};

#endif