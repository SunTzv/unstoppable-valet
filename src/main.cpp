#include "include/car.h"
#include "include/player.h"
#include "include/world.h"
#include <vector>

int main() {
    // 1. Initialization
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
        // --- 2. SPAWNING LOGIC ---
        // Spawn a new car if there are fewer than 8 cars and entrance is clear
        bool entranceClear = true;
        for (auto c : cars) {
            // Check against entrance area (700, 550)
            if (CheckCollisionCircles(c->pos, 60, {700, 550}, 60)) entranceClear = false;
        }

        if (cars.size() < 8 && entranceClear && !player.inCar) {
            Car* newCar = new Car({ 700, 650 }); // Spawn slightly off-screen bottom
            int randColor = GetRandomValue(0, 4);
            newCar->texture = LoadTexture(TextFormat("assets/sprites/%s", carFiles[randColor]));
            newCar->speed = 2.5f; // Initial push to enter the lot
            cars.push_back(newCar);
        }

        // --- 3. PHYSICS & COLLISIONS ---
        
        // Car-to-Car Collision
        for (size_t i = 0; i < cars.size(); i++) {
            for (size_t j = i + 1; j < cars.size(); j++) {
                // Using 45 as a radius for the 64x128 cars
                if (CheckCollisionCircles(cars[i]->pos, 45, cars[j]->pos, 45)) {
                    // Calculate push direction
                    Vector2 dir = Vector2Subtract(cars[i]->pos, cars[j]->pos);
                    dir = Vector2Normalize(dir);
                    
                    // Simple "Jolly" Bounce: Push them away from each other
                    cars[i]->pos = Vector2Add(cars[i]->pos, Vector2Scale(dir, 3.0f));
                    cars[j]->pos = Vector2Subtract(cars[j]->pos, Vector2Scale(dir, 3.0f));
                    
                    // Kill momentum on impact
                    cars[i]->speed *= 0.2f;
                    cars[j]->speed *= 0.2f;
                }
            }
        }

        // --- 4. UPDATE & EXIT LOGIC ---
        player.Update();

        // Safe deletion loop for cars exiting
        for (auto it = cars.begin(); it != cars.end(); ) {
            Car* car = *it;
            car->Update();

            // EXIT LOGIC: If car is in exit zone and NO ONE is driving it
            if (!car->isOccupied && CheckCollisionPointRec(car->pos, lot.exit)) {
                UnloadTexture(car->texture);
                delete car;
                it = cars.erase(it); // Remove from list
            } else {
                ++it;
            }
        }

        // --- 5. INTERACTION (SPACE) ---
        if (IsKeyPressed(KEY_SPACE)) {
            if (player.inCar && occupiedCar != nullptr) {
                // EXITING
                player.inCar = false;
                // Pop out far enough so we don't instantly re-enter (due to large hitboxes)
                player.pos = { occupiedCar->pos.x - 80, occupiedCar->pos.y };
                occupiedCar->isOccupied = false;
                occupiedCar = nullptr;
            } else {
                // ENTERING
                for (auto car : cars) {
                    // Check if player is near the car (distance 50 for the 48px player)
                    if (CheckCollisionCircles(player.pos, 50, car->pos, 50)) {
                        player.inCar = true;
                        car->isOccupied = true;
                        occupiedCar = car;
                        break;
                    }
                }
            }
        }

        // Snap player to car position while driving
        if (player.inCar && occupiedCar != nullptr) {
            player.pos = occupiedCar->pos;
        }

        // --- 6. PARKING SLOT CHECK ---
        for (auto& slot : lot.slots) {
            slot.isOccupied = false;
            for (auto car : cars) {
                // If the center of the car is inside the slot rectangle
                if (CheckCollisionPointRec(car->pos, slot.rect)) {
                    slot.isOccupied = true;
                }
            }
        }

        // --- 7. DRAWING ---
        BeginDrawing();
            lot.Draw(); // Draws background, border, and slots
            
            for (auto car : cars) car->Draw();
            player.Draw();

            // UI Overlays
            DrawRectangle(0, 0, 200, 40, Fade(BLACK, 0.5f));
            DrawText(TextFormat("CARS IN LOT: %d", (int)cars.size()), 15, 10, 20, RAYWHITE);
            
            if (!player.inCar) {
                DrawText("WALK TO A CAR & PRESS [SPACE]", 240, 565, 20, GOLD);
            } else {
                DrawText("PARK IN A SLOT OR TAKE TO EXIT", 240, 565, 20, LIME);
            }
        EndDrawing();
    }

    // --- 8. CLEANUP ---
    UnloadTexture(lot.background);
    UnloadTexture(player.spriteSheet);
    for (auto car : cars) {
        UnloadTexture(car->texture);
        delete car;
    }
    CloseWindow();
    return 0;
}