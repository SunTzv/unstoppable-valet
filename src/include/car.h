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
    float radius = 36;        // Tightened collision radius
    const float maxSpeed =  8.5f;  // Forward top speed
    const float maxReverse = 4.0f; // Reverse top speed
    bool isOccupied;
    Texture2D texture;

    Car(Vector2 startPos) {
        pos = startPos;
        rotation = -90.0f; // Spawns facing North (Up)
        speed = 0;
        isOccupied = false;
        texture.id = 0;
    }

    void Update() {
        if (!isOccupied) {
            // Natural rolling friction — decelerates faster when uncontrolled
            speed *= 0.92f;
        } else {
            // Throttle & brake with separate force values
            if (IsKeyDown(KEY_W)) {
                float throttle = 0.28f * (1.0f - fabsf(speed) / maxSpeed * 0.5f); // Tapers near top speed
                speed += throttle;
            }
            if (IsKeyDown(KEY_S)) {
                if (speed > 0.1f)
                    speed -= 0.32f;          // Braking
                else
                    speed -= 0.14f;          // Reverse
            }

            // Speed cap in both directions
            speed = Clamp(speed, -maxReverse, maxSpeed);

            // Speed-sensitive steering: tight at low speed, stable at high speed
            float absSpd   = fabsf(speed);
            float steerMax = 3.8f;
            float steerMin = 1.4f;
            float steerT   = Clamp(absSpd / maxSpeed, 0.0f, 1.0f);
            float steerPow = steerMax + (steerMin - steerMax) * steerT;

            // Flip steering direction when reversing
            float steerDir = (speed >= 0) ? 1.0f : -1.0f;
            if (IsKeyDown(KEY_A)) rotation -= steerPow * steerDir;
            if (IsKeyDown(KEY_D)) rotation += steerPow * steerDir;

            // Drag — slightly stronger at high speed to create a natural top-speed feel
            float dragBase = 0.982f;
            float dragExtra = 0.006f * steerT;  // Extra drag when fast
            speed *= (dragBase - dragExtra);
        }

        pos.x += cosf(rotation * DEG2RAD) * speed;
        pos.y += sinf(rotation * DEG2RAD) * speed;

        // 32px Border Clamping (Ignore if in exit zone)
        bool inExit = (pos.x > 40 && pos.x < 160 && pos.y > 450);
        if (!inExit) {
            pos.x = Clamp(pos.x, 32 + width/4, SCREEN_W - 32 - width/4);
            pos.y = Clamp(pos.y, 32 + height/4, SCREEN_H - 32 - height/4);
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