#ifndef WORLD_H
#define WORLD_H

#include "common.h"
#include <vector>

struct ParkingSlot {
    Rectangle rect;
    bool isOccupied;
};

class ParkingLot {
public:
    Texture2D background;
    std::vector<ParkingSlot> slots;
    Rectangle entrance;
    Rectangle exit;
    const float border = 32.0f;

    ParkingLot() {
        // Shifted 50px up (Y=500 instead of 550)
        entrance = { 650, 500, 100, 100 };
        exit = { 50, 500, 100, 100 };

        for (int r = 0; r < 6; r++) {
            for (int c = 0; c < 6; c++) {
                slots.push_back({{ 100.0f + (c * 105.0f), 50.0f + (r * 75.0f), 100, 60 }, false});
            }
        }
    }

    void Draw() {
        if (background.id != 0) DrawTexture(background, 0, 0, WHITE);
        else ClearBackground(DARKGRAY);

        // Visual 32px Border
        DrawRectangle(0, 0, SCREEN_W, border, Fade(BLACK, 0.4f));
        DrawRectangle(0, SCREEN_H - border, SCREEN_W, border, Fade(BLACK, 0.4f));
        DrawRectangle(0, 0, border, SCREEN_H, Fade(BLACK, 0.4f));
        DrawRectangle(SCREEN_W - border, 0, border, SCREEN_H, Fade(BLACK, 0.4f));

        DrawRectangleRec(entrance, Fade(LIME, 0.3f));
        DrawRectangleRec(exit, Fade(RED, 0.3f));
        DrawText("ENTER", entrance.x + 20, entrance.y + 40, 18, WHITE);
        DrawText("EXIT", exit.x + 28, exit.y + 40, 18, WHITE);
        
        // Green debug boxes removed for clean gameplay
    }
};

#endif