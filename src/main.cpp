#include "include/car.h"
#include "include/player.h"

int main() {
    InitWindow(SCREEN_W, SCREEN_H, "UNSTOPPABLE VALET");
    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(VIRTUAL_W, VIRTUAL_H);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    Player player;
    // Create a few test cars
    Car testCar({ 50, 50 }, MEDIUM, MAROON);
    Car truck({ 150, 100 }, LARGE, LIME);

    // --- ASSET LOADING ---
    // These will fail gracefully if files don't exist yet
    player.texture = LoadTexture("assets/sprites/valet.png");
    testCar.texture = LoadTexture("assets/sprites/car_med.png");
    truck.texture = LoadTexture("assets/sprites/car_large.png");

    while (!WindowShouldClose()) {
        // 1. UPDATE
        player.Update();
        
        // Check for Car Interaction
        if (IsKeyPressed(KEY_SPACE)) {
            if (player.inCar) {
                player.inCar = false;
                // Position player next to car when exiting
                player.pos = { testCar.pos.x + 15, testCar.pos.y }; 
                testCar.isOccupied = false;
            } else {
                // Check if near the sedan
                if (CheckCollisionCircles(player.pos, 5, testCar.pos, 10)) {
                    player.inCar = true;
                    testCar.isOccupied = true;
                }
            }
        }

        testCar.Update();
        truck.Update();

        if (player.inCar) player.pos = testCar.pos; // Follow the car

        // 2. DRAW
        BeginTextureMode(target);
            ClearBackground(Color{ 50, 50, 55, 255 });
            
            // Draw UI Hint
            DrawText("SPACE to Enter/Exit", 5, 5, 5, RAYWHITE);

            testCar.Draw();
            truck.Draw();
            player.Draw();
        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(target.texture, 
                { 0, 0, (float)target.texture.width, (float)-target.texture.height }, 
                { 0, 0, (float)SCREEN_W, (float)SCREEN_H }, 
                { 0, 0 }, 0, WHITE);
        EndDrawing();
    }

    // Cleanup
    UnloadTexture(player.texture);
    UnloadTexture(testCar.texture);
    UnloadTexture(truck.texture);
    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}