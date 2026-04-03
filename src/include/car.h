#ifndef CAR_H
#define CAR_H

#include "common.h"
#include "raymath.h"

class Car {
public:
    Vector2 pos;
    float rotation;
    float speed;
    CarSize size;
    Color color;
    bool isOccupied;
    Texture2D texture;

    Car(Vector2 startPos, CarSize s, Color c) {
        pos = startPos;
        rotation = 0;
        speed = 0;
        size = s;
        color = c;
        isOccupied = false;
    }

    void Update() {
        if (!isOccupied) {
            speed *= 0.9f; // Friction when empty
        } else {
            // Driving Logic
            if (IsKeyDown(KEY_W)) speed += 0.05f;
            if (IsKeyDown(KEY_S)) speed -= 0.03f;
            
            if (speed != 0) {
                if (IsKeyDown(KEY_A)) rotation -= 3.0f;
                if (IsKeyDown(KEY_D)) rotation += 3.0f;
            }
            speed *= 0.98f; // Air resistance
        }

        // Move based on rotation (Trigonometry!)
        pos.x += cosf(rotation * DEG2RAD) * speed;
        pos.y += sinf(rotation * DEG2RAD) * speed;
    }

    void Draw() {
        Rectangle source = { 0, 0, (float)size, (float)size };
        Rectangle dest = { pos.x, pos.y, (float)size, (float)size };
        Vector2 origin = { (float)size / 2, (float)size / 2 };

        if (texture.id != 0) {
            DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
        } else {
            // Fallback: Draw a box if no texture loaded
            DrawRectanglePro(dest, origin, rotation, color);
        }
    }
};

#endif