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
    
    lot.background = LoadTexture("assets/sprites/parking_lot.png");
    player.spriteSheet = LoadTexture("assets/sprites/valet_sheet.png");
    const char* carFiles[] = {"cyan.png", "green.png", "orange.png", "pink.png", "yellow.png"};

    Car* occupiedCar = nullptr;

    while (!WindowShouldClose()) {
        // --- SPAWNING ---
        bool entranceClear = true;
        for (auto c : cars) if (CheckCollisionCircles(c->pos, 65, {700, 550}, 60)) entranceClear = false;

        if (cars.size() < 8 && entranceClear && !player.inCar) {
            Car* newCar = new Car({ 700, 650 }); // Spawn off-screen
            newCar->texture = LoadTexture(TextFormat("assets/sprites/%s", carFiles[GetRandomValue(0, 4)]));
            newCar->speed = 3.0f; // Drive in
            cars.push_back(newCar);
        }

        // --- UPDATE & COLLISIONS ---
        player.Update();

        // Player-to-Car Collision — hard stop: cancel movement into the car
        if (!player.inCar) {
            for (auto car : cars) {
                const float playerR = 14.0f;
                const float carR    = 32.0f;
                float dist = Vector2Distance(player.pos, car->pos);
                float overlap = (playerR + carR) - dist;
                if (overlap > 0) {
                    // Push player exactly out of overlap along the normal
                    Vector2 normal = Vector2Normalize(Vector2Subtract(player.pos, car->pos));
                    player.pos = Vector2Add(player.pos, Vector2Scale(normal, overlap + 0.5f));
                }
            }
        }

        // Car-to-Car Collision — proper momentum exchange
        for (size_t i = 0; i < cars.size(); i++) {
            for (size_t j = i + 1; j < cars.size(); j++) {
                const float colR = 36.0f;
                float dist = Vector2Distance(cars[i]->pos, cars[j]->pos);
                float overlap = (colR + colR) - dist;
                if (overlap > 0) {
                    Vector2 normal = Vector2Normalize(Vector2Subtract(cars[i]->pos, cars[j]->pos));

                    // Separate the two cars cleanly (half each)
                    cars[i]->pos = Vector2Add(cars[i]->pos, Vector2Scale(normal, overlap * 0.5f + 1.0f));
                    cars[j]->pos = Vector2Subtract(cars[j]->pos, Vector2Scale(normal, overlap * 0.5f + 1.0f));

                    // Decompose both velocities along the collision normal
                    Vector2 velI = { cosf(cars[i]->rotation * DEG2RAD) * cars[i]->speed,
                                     sinf(cars[i]->rotation * DEG2RAD) * cars[i]->speed };
                    Vector2 velJ = { cosf(cars[j]->rotation * DEG2RAD) * cars[j]->speed,
                                     sinf(cars[j]->rotation * DEG2RAD) * cars[j]->speed };

                    float vI = Vector2DotProduct(velI, normal);
                    float vJ = Vector2DotProduct(velJ, normal);

                    // Elastic-ish exchange with restitution (0.55 = partial bounce)
                    const float restitution = 0.55f;
                    float impulse = -(1.0f + restitution) * (vI - vJ) * 0.5f;

                    // Apply speed change as a scalar nudge after dampening
                    cars[i]->speed += impulse * 0.9f;
                    cars[j]->speed -= impulse * 0.9f;

                    // Dampen both a bit to avoid perpetual bouncing
                    cars[i]->speed *= 0.75f;
                    cars[j]->speed *= 0.75f;
                }
            }
        }

        // Car Logic & Exit Deletion
        for (auto it = cars.begin(); it != cars.end(); ) {
            (*it)->Update();
            if (!(*it)->isOccupied && CheckCollisionPointRec((*it)->pos, lot.exit)) {
                UnloadTexture((*it)->texture);
                delete *it;
                it = cars.erase(it);
            } else { ++it; }
        }

        // Interaction
        if (IsKeyPressed(KEY_SPACE)) {
            if (player.inCar && occupiedCar != nullptr) {
                player.inCar = false;
                player.pos = { occupiedCar->pos.x - 85, occupiedCar->pos.y };
                occupiedCar->isOccupied = false;
                occupiedCar = nullptr;
            } else {
                for (auto car : cars) {
                    if (CheckCollisionCircles(player.pos, 55, car->pos, 50)) {
                        player.inCar = true;
                        car->isOccupied = true;
                        occupiedCar = car;
                        break;
                    }
                }
            }
        }

        if (player.inCar && occupiedCar != nullptr) player.pos = occupiedCar->pos;

        // Sync invisible slots
        for (auto& slot : lot.slots) {
            slot.isOccupied = false;
            for (auto car : cars) if (CheckCollisionPointRec(car->pos, slot.rect)) slot.isOccupied = true;
        }

        // --- DRAW ---
        BeginDrawing();
            lot.Draw();
            for (auto car : cars) car->Draw();
            player.Draw();
            
            DrawRectangle(10, 10, 180, 35, Fade(BLACK, 0.6f));
            DrawText(TextFormat("CARS IN LOT: %d", (int)cars.size()), 20, 18, 18, RAYWHITE);
        EndDrawing();
    }

    // Cleanup
    UnloadTexture(lot.background);
    UnloadTexture(player.spriteSheet);
    for (auto car : cars) { UnloadTexture(car->texture); delete car; }
    CloseWindow();
    return 0;
}