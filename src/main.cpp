#include "include/car.h"
#include "include/player.h"
#include "include/world.h"
#include <vector>

int main() {
    InitWindow(SCREEN_W, SCREEN_H, "UNSTOPPABLE VALET");
    SetTargetFPS(60);

    Player player;
    ParkingLot lot;
    std::vector<Car*> cars;
    
    // Load Textures
    lot.background = LoadTexture("assets/sprites/parking_lot.png");
    player.spriteSheet = LoadTexture("assets/sprites/valet_sheet.png");
    
    const char* carFiles[] = {"cyan.png", "green.png", "orange.png", "pink.png", "yellow.png"};
    Car* occupiedCar = nullptr;

    while (!WindowShouldClose()) {
        // --- SPAWNING LOGIC ---
        // Spawn a new car if there are fewer than 10 cars and entrance is clear
        bool entranceClear = true;
        for (auto c : cars) {
            if (CheckCollisionCircles(c->pos, 50, {700, 550}, 60)) entranceClear = false;
        }

        if (cars.size() < 10 && entranceClear && !player.inCar) {
            Car* newCar = new Car({ 700, 650 }); // Spawn slightly off-screen bottom
            int randColor = GetRandomValue(0, 4);
            newCar->texture = LoadTexture(TextFormat("assets/sprites/%s", carFiles[randColor]));
            newCar->speed = 2.0f; // Make it drive in automatically
            cars.push_back(newCar);
        }

        // --- UPDATE ---
        player.Update();
        for (auto car : cars) car->Update();

        // Interaction (Space)
        if (IsKeyPressed(KEY_SPACE)) {
            if (player.inCar && occupiedCar != nullptr) {
                player.inCar = false;
                player.pos = { occupiedCar->pos.x - 60, occupiedCar->pos.y };
                occupiedCar->isOccupied = false;
                occupiedCar = nullptr;
            } else {
                for (auto car : cars) {
                    if (CheckCollisionCircles(player.pos, 45, car->pos, 45)) {
                        player.inCar = true;
                        car->isOccupied = true;
                        occupiedCar = car;
                        break;
                    }
                }
            }
        }

        // Check Slots
        for (auto& slot : lot.slots) {
            slot.isOccupied = false;
            for (auto car : cars) {
                if (CheckCollisionPointRec(car->pos, slot.rect)) slot.isOccupied = true;
            }
        }

        if (player.inCar && occupiedCar != nullptr) player.pos = occupiedCar->pos;

        // --- DRAW ---
        BeginDrawing();
            lot.Draw();
            for (auto car : cars) car->Draw();
            player.Draw();

            DrawText(TextFormat("Cars in Lot: %d", (int)cars.size()), 10, 10, 20, RAYWHITE);
            if (!player.inCar) DrawText("Walk to a car and press SPACE", 250, 570, 20, GOLD);
        EndDrawing();
    }

    // Cleanup
    UnloadTexture(lot.background);
    UnloadTexture(player.spriteSheet);
    for (auto car : cars) {
        UnloadTexture(car->texture);
        delete car;
    }
    CloseWindow();
    return 0;
}