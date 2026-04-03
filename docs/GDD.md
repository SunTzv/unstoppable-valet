## 🅿️ Game Design Document: Unstoppable Valet

### 1. Game Overview
**Unstoppable Valet** is a minimalist, top-down parking simulator. The player acts as a valet in a small, colorful lot. The goal is to efficiently manage space by parking incoming cars and retrieving specific ones when owners return. The game focuses on "cozy repetition" and "juicy" feedback.

* **Internal Resolution:** 200 x 150 pixels
* **Target Window:** 800 x 600 pixels (4x Integer Scaling)
* **Style:** Low-fidelity pixel art, high-saturation colors, "ploppy" physics.

---

### 2. Core Gameplay Loop
1.  **The Arrival:** A car appears at the "Entrance" zone with a specific color/size.
2.  **The Task:** A UI bubble shows: "PARK" or "RETRIEVE [ID/Color]".
3.  **The Maneuver:** The player drives the car into an empty spot. 
4.  **The Reward:** A "Jolly" SFX plays, and the lot fills up.
5.  **The Retrieval:** A customer returns. The player must navigate the lot (potentially moving other cars) to get the requested car to the "Exit" zone.

---

### 3. Mechanics & Controls

#### **The Cars**
Cars come in three distinct "Classes":
| Class | Size (px) | Handling |
| :--- | :--- | :--- |
| **Compact** | 8 x 8 | High acceleration, sharp turning. |
| **Sedan** | 12 x 12 | Standard handling. |
| **Truck** | 16 x 16 | Slow, wide turning circle, harder to fit. |

#### **Movement**
* **Top-down Tank Controls:** Up/Down for Gas/Brake, Left/Right for Steering.
* **Soft Collisions:** Bumping into walls or other cars doesn't end the game; it just produces a "boing" sound and a tiny screen shake.

---

### 4. Technical Specifications (Raylib)

#### **The Upscaling Technique**
To achieve the 200x150 look on an 800x600 screen, we use a **Render Texture**. 
1. Draw everything to a `RenderTexture2D` at 200x150.
2. Draw that texture to the screen at 800x600 using `DrawTexturePro()` with a 4x scale.
3. Use `SetTextureFilter(target.texture, TEXTURE_FILTER_POINT)` to keep the pixels sharp.

#### **The Map Grid**
The 200x150 play area will be divided into:
* **Driving Lanes:** 40px wide.
* **Parking Bays:** Designated rectangles (e.g., 20x20px) to accommodate the 16x16 trucks.
* **Zones:** 1 Entrance, 1 Exit.

---

### 5. Aesthetics (The "Jolly" Factor)

* **Visuals:** * No black outlines. Use "shaded" colors (e.g., a dark blue car with a light blue roof).
    * When a car is parked correctly, it should "squash and stretch" (scale up and down slightly) for a second.
* **Audio:** * **Engine:** A low-bitrate "purr."
    * **Success:** A major-chord chime or a "Happy Whistle."
    * **Ambience:** Distant birds chirping or elevator-style lo-fi music.

---

### 6. Progression & "Game Over"
* **Victory:** The lot is 100% full (all spots occupied). A "Lot Full!" celebration occurs, and the board resets or increases in difficulty (tighter spots).
* **Challenge:** As the lot fills, the player has less room to turn. The "Puzzle" comes from unparking a car buried deep in the back.

---