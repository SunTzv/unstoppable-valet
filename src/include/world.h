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

    ParkingLot() {
        // Entrance: Bottom Right (700, 600 area)
        entrance = { 650, 500, 150, 100 };
        // Exit: Bottom Left
        exit = { 0, 500, 150, 100 };

        // Define 6 horizontal lanes based on your image
        // Lanes are approx 70px high with some padding
        for (int row = 0; row < 6; row++) {
            for (int col = 0; col < 6; col++) {
                // Parking slots are 64x128, but your lanes are horizontal.
                // We'll rotate the slots to be 128x64 to fit the lanes!
                float x = 100.0f + (col * 105.0f);
                float y = 50.0f + (row * 75.0f);
                slots.push_back({{ x, y, 100, 60 }, false});
            }
        }
    }

    void Draw() {
        if (background.id != 0) {
            DrawTexture(background, 0, 0, WHITE);
        } else {
            ClearBackground(DARKGRAY); // Fallback
        }

        // Draw Slots (Debug view - remove later)
        for (auto& slot : slots) {
            DrawRectangleLinesEx(slot.rect, 1, slot.isOccupied ? RED : Fade(GREEN, 0.5f));
        }
    }
};

#endif