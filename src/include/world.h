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
    const float borderThickness = 32.0f;

    ParkingLot() {
        // Entrance: Centered around 700, 600 (Bottom Right)
        entrance = { 650, 550, 100, 100 };
        
        // Exit: Centered around 100, 600 (Bottom Left)
        exit = { 50, 550, 100, 100 };

        // Define 6 horizontal lanes (matching your 800x600 asset)
        // We start drawing slots inside the 32px border
        for (int row = 0; row < 6; row++) {
            for (int col = 0; col < 6; col++) {
                // Slots are 100x60 to fit horizontal lanes comfortably
                float x = 100.0f + (col * 105.0f);
                float y = 50.0f + (row * 75.0f);
                slots.push_back({{ x, y, 100, 60 }, false});
            }
        }
    }

    void Draw() {
        // 1. Draw Background Asset
        if (background.id != 0) {
            DrawTexture(background, 0, 0, WHITE);
        } else {
            ClearBackground(DARKGRAY); 
        }

        // 2. Draw 32px Border (Visual guide for the "wall")
        // We draw 4 rectangles for the sides
        DrawRectangle(0, 0, SCREEN_W, borderThickness, Fade(BLACK, 0.3f)); // Top
        DrawRectangle(0, SCREEN_H - borderThickness, SCREEN_W, borderThickness, Fade(BLACK, 0.3f)); // Bottom
        DrawRectangle(0, 0, borderThickness, SCREEN_H, Fade(BLACK, 0.3f)); // Left
        DrawRectangle(SCREEN_W - borderThickness, 0, borderThickness, SCREEN_H, Fade(BLACK, 0.3f)); // Right

        // 3. Draw Entry/Exit Zones
        DrawRectangleRec(entrance, Fade(LIME, 0.4f));
        DrawRectangleRec(exit, Fade(RED, 0.4f));
        
        DrawText("ENTER", entrance.x + 20, entrance.y + 40, 20, WHITE);
        DrawText("EXIT", exit.x + 25, exit.y + 40, 20, WHITE);

        // 4. Draw Slots (Debug lines)
        for (auto& slot : slots) {
            // Slots turn Red if a car is detected inside
            DrawRectangleLinesEx(slot.rect, 2, slot.isOccupied ? RED : Fade(GREEN, 0.5f));
        }
    }
};

#endif