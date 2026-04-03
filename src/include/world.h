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
        // Entrance: Moved 50px up (was 550, now 500)
        entrance = { 650, 500, 100, 100 };
        
        // Exit: Moved 50px up (was 550, now 500)
        exit = { 50, 500, 100, 100 };

        for (int row = 0; row < 6; row++) {
            for (int col = 0; col < 6; col++) {
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
            ClearBackground(DARKGRAY); 
        }

        // Visual Border (The 32px "Wall")
        DrawRectangle(0, 0, SCREEN_W, borderThickness, Fade(BLACK, 0.4f)); 
        DrawRectangle(0, SCREEN_H - borderThickness, SCREEN_W, borderThickness, Fade(BLACK, 0.4f)); 
        DrawRectangle(0, 0, borderThickness, SCREEN_H, Fade(BLACK, 0.4f)); 
        DrawRectangle(SCREEN_W - borderThickness, 0, borderThickness, SCREEN_H, Fade(BLACK, 0.4f)); 

        // Draw Entry/Exit Zones (Shifted Up)
        DrawRectangleRec(entrance, Fade(LIME, 0.4f));
        DrawRectangleRec(exit, Fade(RED, 0.4f));
        
        DrawText("ENTER", entrance.x + 20, entrance.y + 40, 20, WHITE);
        DrawText("EXIT", exit.x + 25, exit.y + 40, 20, WHITE);

        // NOTE: Green debug boxes for slots have been removed from Draw()
    }
};

#endif