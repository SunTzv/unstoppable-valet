#include "include/car.h"
#include "include/player.h"
#include <vector>

int main() {
    InitWindow(SCREEN_W, SCREEN_H, "UNSTOPPABLE VALET");
    SetTargetFPS(60);

    Player player;
    std::vector<Car*> parkingLot;

    // Load assets
    player.spriteSheet = LoadTexture("assets/sprites/valet_sheet.png");

    // Let's create the starting cars
    const char* colors[] = {"cyan.png", "green.png", "orange.png", "pink.png", "yellow.png"};
    for(int i = 0; i < 5; i++) {
        Car* c = new Car({ (float)(150 + i * 120), 300.0f });
        c->texture = LoadTexture(TextFormat("assets/sprites/%s", colors[i]));
        parkingLot.push_back(c);
    }

    Car* occupiedCar = nullptr;

    while (!WindowShouldClose()) {
        // --- UPDATE ---
        player.Update();

        if (IsKeyPressed(KEY_SPACE)) {
            if (player.inCar && occupiedCar != nullptr) {
                player.inCar = false;
                player.pos = { occupiedCar->pos.x - 80, occupiedCar->pos.y }; // Hop out clear of the big car
                occupiedCar->isOccupied = false;
                occupiedCar = nullptr;
            } else {
                for (auto car : parkingLot) {
                    // Check interaction near the big car hitbox
                    if (CheckCollisionCircles(player.pos, 40, car->pos, 40)) {
                        player.inCar = true;
                        car->isOccupied = true;
                        occupiedCar = car;
                        break;
                    }
                }
            }
        }

        for (auto car : parkingLot) car->Update();
        if (player.inCar && occupiedCar != nullptr) player.pos = occupiedCar->pos;

        // --- DRAWING ---
        BeginDrawing();
            ClearBackground(DARKGRAY); // Your background color/texture goes here
            
            for (auto car : parkingLot) car->Draw();
            player.Draw();

            DrawText("UNSTOPPABLE VALET", 10, 10, 20, RAYWHITE);
        EndDrawing();
    }

    // Cleanup
    UnloadTexture(player.spriteSheet);
    for (auto car : parkingLot) {
        UnloadTexture(car->texture);
        delete car;
    }
    CloseWindow();
    return 0;
}