#include <raylib.h>
#include <cmath>
#include "Phys.h"
#include "Entity.h"
#include <string>
#include <vector>


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Game State Enum
enum GameState {
    PLAYING,
    GAME_OVER
};

float calculateLaunchAngle(Vector2 throwerPos, Vector2 mousePos, bool isRedTurn) {
    float angle = std::atan2(throwerPos.y - mousePos.y, mousePos.x - throwerPos.x) * (180.0f / M_PI);
    // Invert
    angle += 180.0f;
    if (angle >= 360.0f) angle -= 360.0f;

    // For blue entity, invert the angle
    if (!isRedTurn) {
        angle += 360.0f;
    }

    if (angle >= 360.0f) angle -= 360.0f;

    return angle;
}

Entity spawnRandomPolygon(int screenWidth, int screenHeight) {
    // Possible polygon types
    std::vector<void (Entity::*)() const> polygonTypes = {
        &Entity::drawTriangle,
        &Entity::drawRectangle,
        &Entity::drawPentagon,
        &Entity::drawHexagon
    };

        std::vector<Entity::EntityType> entityTypes = {
        Entity::EntityType::TRIANGLE,
        Entity::EntityType::RECTANGLE,
        Entity::EntityType::PENTAGON,
        Entity::EntityType::HEXAGON
    };

    // Randomly select polygon type
    int polygonIndex = GetRandomValue(0, polygonTypes.size() - 1);

    // Randomly position the polygon
    Vector2 position = {
        static_cast<float>(GetRandomValue(100, screenWidth - 100)),
        static_cast<float>(GetRandomValue(100, screenHeight - 100))
    };

    // Create an entity with fixed width and height, random color
    Color randomColor = {
        static_cast<unsigned char>(GetRandomValue(50, 200)),
        static_cast<unsigned char>(GetRandomValue(50, 200)),
        static_cast<unsigned char>(GetRandomValue(50, 200)),
        255
    };

    Entity randomPolygon(position, 40.0f, 40.0f, randomColor, entityTypes[polygonIndex]);
    randomPolygon.polygonType = polygonTypes[polygonIndex];

        switch(polygonIndex) {
        case 0: // Triangle
            randomPolygon.setTexture("src/img/SBS - 2D Planet Pack - Large 512x512/Large 512x512/Radiated/Radiated_02-512x512.png", Entity::EntityType::TRIANGLE);
            break;
        case 1: // Rectangle
            randomPolygon.setTexture("src/img/SBS - 2D Planet Pack - Large 512x512/Large 512x512/Ocean - Clouds/Ocean_Clouds_01-512x512.png", Entity::EntityType::RECTANGLE);
            break;
        case 2: // Pentagon
            randomPolygon.setTexture("src/img/SBS - 2D Planet Pack - Large 512x512/Large 512x512/Desert/Desert_03-512x512.png", Entity::EntityType::PENTAGON);
            break;
        case 3: // Hexagon
            randomPolygon.setTexture("src/img/SBS - 2D Planet Pack - Large 512x512/Large 512x512/Gas - Giant/Gas_Giant_06-512x512.png", Entity::EntityType::HEXAGON);
            break;
    }

    return randomPolygon;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Galactic Baseballer");
    SetTargetFPS(144);

    Texture2D backgroundTexture = LoadTexture("src/img/space_background_pack/space_background_pack/Assets/Blue Version/blue-preview.png");
    Texture2D paddle1Texture = LoadTexture("src/img/spaceships/ships/gray3.png");
    Texture2D paddle2Texture = LoadTexture("src/img/spaceships/ships/green.png");
    Texture2D winScreenTexture = LoadTexture("src/img/UI/winner_screen.png");
    Texture2D rematchBtnTexture = LoadTexture("src/img/Buttons-PNG/PNG/shiny/3.png");
    Texture2D exitBtnTexture = LoadTexture("src/img/Buttons-PNG/PNG/shiny/11.png");


    // Colors
    Color BACKGROUND_COLOR = {0, 255, 255, 128}; // Cyan
    Color THROWER_COLOR1 = {255, 0, 0, 255};     // Red
    Color THROWER_COLOR2 = {0, 0, 255, 255};     // Blue

    // Phys for Ball
    Physics ball({128, 625}, 0, 0, 9.8f, 0.0f);
    ball.setTexture("src/img/Planets/Black_hole.png");
    bool ballInAir = false;

    // Thrower Pos with HP
    Entity thrower1({128, 625}, 60, 60, THROWER_COLOR1, Entity::EntityType::DEFAULT);
    Entity thrower2({1152, 625}, 60, 60, THROWER_COLOR2, Entity::EntityType::DEFAULT);

    thrower1.setTexture("src/img/Planets/Lava.png", Entity::EntityType::DEFAULT);
    thrower2.setTexture("src/img/Planets/Ice.png", Entity::EntityType::DEFAULT);
    
    // Add HP to entities
    int thrower1HP = 5;
    int thrower2HP = 5;

    // Turn-based mechanics
    bool isRedTurn = true;
    Entity* currentThrower = &thrower1;
    Physics* currentBall = &ball;

    // Game State
    GameState gameState = PLAYING;

    // Throw mechanic
    float mouseHoldDuration = 0.0f;
    bool isMouseHeld = false;
    float launchAngle = 90.0f;

    // Collision Timer
    float collisionTimer = -1.0f; // -1 to indicate no timer is active
    bool canCheckCollision = false; // Flag for enabling collision checks

    // Paddle orbiting variables
    float paddleOrbitRadius1 = 50.0f;  // Orbit radius for Thrower1's paddle
    float paddleOrbitAngle1 = 0.0f;    // Current orbit angle for Thrower1's paddle
    float paddleOrbitSpeed1 = 2.0f;    // Orbit speed for Thrower1's paddle

    float paddleOrbitRadius2 = 50.0f;  // Orbit radius for Thrower2's paddle
    float paddleOrbitAngle2 = 0.0f;    // Current orbit angle for Thrower2's paddle
    float paddleOrbitSpeed2 = 2.0f;    // Orbit speed for Thrower2's paddle

    float paddleWidth = 6.0f;  // You might want to adjust these based on your textures
    float paddleHeight = 20.0f;
    float polygonSpawnTimer = 0.0f;
    const float POLYGON_SPAWN_INTERVAL = 7.0f;
    std::vector<Entity> activePolygons;

    while (!WindowShouldClose()) {
        if (gameState == PLAYING) {
            // Paddle Orbit Controls for Thrower1 (WASD)
            if (IsKeyDown(KEY_A)) {
                paddleOrbitAngle1 -= paddleOrbitSpeed1;
            }
            if (IsKeyDown(KEY_D)) {
                paddleOrbitAngle1 += paddleOrbitSpeed1;
            }
            if (IsKeyDown(KEY_W)) {
                // Increase orbit radius
                paddleOrbitRadius1 = std::min(paddleOrbitRadius1 + 1.0f, 100.0f);
            }
            if (IsKeyDown(KEY_S)) {
                // Decrease orbit radius
                paddleOrbitRadius1 = std::max(paddleOrbitRadius1 - 1.0f, 20.0f);
            }

            // Paddle Orbit Controls for Thrower2 (Arrow Keys)
            if (IsKeyDown(KEY_LEFT)) {
                paddleOrbitAngle2 -= paddleOrbitSpeed2;
            }
            if (IsKeyDown(KEY_RIGHT)) {
                paddleOrbitAngle2 += paddleOrbitSpeed2;
            }
            if (IsKeyDown(KEY_UP)) {
                // Increase orbit radius
                paddleOrbitRadius2 = std::min(paddleOrbitRadius2 + 1.0f, 100.0f);
            }
            if (IsKeyDown(KEY_DOWN)) {
                // Decrease orbit radius
                paddleOrbitRadius2 = std::max(paddleOrbitRadius2 - 1.0f, 20.0f);
            }

            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                if (!isMouseHeld) {
                    mouseHoldDuration = 0.0f;
                    isMouseHeld = true;
                }

                // Cap 3000ms Hold
                mouseHoldDuration += GetFrameTime() * 1000;
                mouseHoldDuration = std::min(mouseHoldDuration, 3000.0f);
            } else if (isMouseHeld && currentBall->isReadyToLaunch()) {

                Vector2 mousePos = GetMousePosition();
                Vector2 currentThrowerPos = currentThrower->getPosition();

                float force = 1000.0f + (mouseHoldDuration / 1000.0f) * 1000.0f;

                float angle = calculateLaunchAngle(currentThrowerPos, mousePos, isRedTurn);

                isMouseHeld = false;

                // Launch method
                currentBall->launch(force, angle);
                ballInAir = true;

                // Collision timer
                collisionTimer = 0.0f;
                canCheckCollision = false; // Reset the collision check flag
            }

            float deltaTime = GetFrameTime();

            polygonSpawnTimer += deltaTime;
            if (polygonSpawnTimer >= POLYGON_SPAWN_INTERVAL) {
                // Spawn new polygon and reset timer
                activePolygons.clear(); // Remove prev polygon
                activePolygons.push_back(spawnRandomPolygon(SCREEN_WIDTH, SCREEN_HEIGHT));
                polygonSpawnTimer = 0.0f;
            }

            if (ballInAir) {
                currentBall->update(deltaTime, SCREEN_WIDTH, SCREEN_HEIGHT);

                // Update the collision timer
                if (collisionTimer >= 0.0f) {
                    collisionTimer += deltaTime;
                    if (collisionTimer >= 0.5f) { // 500ms after launch
                        canCheckCollision = true; // Enable collision checks
                    }
                }

                // Check for collision with throwers after 500ms
                if (canCheckCollision) {

                    Vector2 thrower1Pos = thrower1.getPosition();
                    Vector2 thrower2Pos = thrower2.getPosition();

                    Vector2 paddlePos1 = {
                        thrower1Pos.x + std::cos(paddleOrbitAngle1 * DEG2RAD) * paddleOrbitRadius1,
                        thrower1Pos.y - std::sin(paddleOrbitAngle1 * DEG2RAD) * paddleOrbitRadius1
                    };

                    Vector2 paddlePos2 = {
                        thrower2Pos.x - std::cos(paddleOrbitAngle2 * DEG2RAD) * paddleOrbitRadius2,
                        thrower2Pos.y + std::sin(paddleOrbitAngle2 * DEG2RAD) * paddleOrbitRadius2
                    };

                    bool paddleHit = false;
                    if (thrower1.checkPaddleCollision(paddlePos1, paddleWidth, paddleHeight, paddleOrbitAngle1 + 90, *currentBall)) {
                        isRedTurn = !isRedTurn;  // Switch turn
                        currentThrower = isRedTurn ? &thrower1 : &thrower2;
                        paddleHit = true;
                    } else if (thrower2.checkPaddleCollision(paddlePos2, paddleWidth, paddleHeight, paddleOrbitAngle2 + 90, *currentBall)) {
                        isRedTurn = !isRedTurn;  // Switch turn
                        currentThrower = isRedTurn ? &thrower1 : &thrower2;
                        paddleHit = true;
                    }

                    if (!paddleHit) {
                        if (thrower1.checkCollision(*currentBall)) {
                            // Reduce HP for thrower1
                            thrower1HP--;
                            currentBall->velocity = {0.0f, 0.0f};  // Stop the ball
                            currentBall->reset();  // Reset ball position
                            currentBall->resetTrajectoryTime(); // Reset trajectory time
                            ballInAir = false;
                            isRedTurn = !isRedTurn;  // Switch turn
                            currentThrower = isRedTurn ? &thrower1 : &thrower2;

                            // Check if game is over
                            if (thrower1HP <= 0) {
                                gameState = GAME_OVER;
                            }
                        } else if (thrower2.checkCollision(*currentBall)) {
                            // Reduce HP for thrower2
                            thrower2HP--;
                            currentBall->velocity = {0.0f, 0.0f};  // Stop the ball
                            currentBall->reset();  // Reset ball position
                            currentBall->resetTrajectoryTime(); // Reset trajectory time
                            ballInAir = false;
                            isRedTurn = !isRedTurn;  // Switch turn
                            currentThrower = isRedTurn ? &thrower1 : &thrower2;

                            // Check if game is over
                            if (thrower2HP <= 0) {
                                gameState = GAME_OVER;
                            }
                        }
                    }

                    // Check if ball movement stopped or out of bounds
                    if (currentBall->isReadyToLaunch() || currentBall->isOutOfBounds(SCREEN_WIDTH, SCREEN_HEIGHT)) {
                        ballInAir = false;

                        // Reset ball and set its position to the respective thrower's position
                        currentBall->reset();
                        if (isRedTurn) {
                            currentThrower = &thrower1;
                            currentBall->position = thrower1.getPosition(); // Set ball position to thrower1's position
                        } else {
                            currentThrower = &thrower2;
                            currentBall->position = thrower2.getPosition(); // Set ball position to thrower2's position
                        }
                        currentBall->resetTrajectoryTime();
                    }
                }

                if (currentBall->isOutOfBounds(SCREEN_WIDTH, SCREEN_HEIGHT)) {
                    ballInAir = false;

                    // Switch turns
                    isRedTurn = !isRedTurn;

                    // Reset ball and set its position to the respective thrower's position
                    currentBall->reset();

                    if (isRedTurn) {
                        currentThrower = &thrower1;
                        currentBall->position = thrower1.getPosition();
                        currentBall->color = THROWER_COLOR1;
                    } else {
                        currentThrower = &thrower2;
                        currentBall->position = thrower2.getPosition();
                        currentBall->color = THROWER_COLOR2;
                    }
                }
            }

            // Drawing
            BeginDrawing();
            DrawTexturePro(
    backgroundTexture,
    (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
        (Rectangle){0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT},
        (Vector2){0, 0},
        0,
        WHITE);
            // Draw game elements
            thrower1.draw();
            thrower2.draw();
            currentBall->draw();

            for (const auto& polygon : activePolygons) {
                if (polygon.polygonType) {
                    // Call the specific polygon drawing method
                    (polygon.*polygon.polygonType)();
                }
            }

            // Turn Indicator
            std::string turnText = isRedTurn ? "Red's Turn" : "Blue's Turn";
            DrawText(turnText.c_str(), 590, 20, 20, isRedTurn ? RED : BLUE);

            // HP Indicator
            std::string redHPText = "Red HP: " + std::to_string(thrower1HP);
            std::string blueHPText = "Blue HP: " + std::to_string(thrower2HP);
            DrawText(redHPText.c_str(), 20, 20, 20, RED);
            DrawText(blueHPText.c_str(), 1150, 20, 20, BLUE);

            // Hold Gauge
            DrawText("Hold mouse to throw", 540, 50, 20, WHITE);
            DrawRectangle(540, 80, 200, 20, GRAY);
            DrawRectangle(540, 80, static_cast<int>(mouseHoldDuration * 200.0f / 3000.0f), 20, GREEN);

            Vector2 thrower1Pos = thrower1.getPosition();
            Vector2 thrower2Pos = thrower2.getPosition();

            // Orbiting Paddle
            Vector2 paddlePos1 = {
                thrower1Pos.x + std::cos(paddleOrbitAngle1 * DEG2RAD) * paddleOrbitRadius1,
                thrower1Pos.y - std::sin(paddleOrbitAngle1 * DEG2RAD) * paddleOrbitRadius1
            };

            Vector2 paddlePos2 = {
                thrower2Pos.x - std::cos(paddleOrbitAngle2 * DEG2RAD) * paddleOrbitRadius2,
                thrower2Pos.y + std::sin(paddleOrbitAngle2 * DEG2RAD) * paddleOrbitRadius2
            };
// Paddle1
if (paddle1Texture.id > 0) {
        float scaleFactor = 20.0f; // Adjust this value to change the scaling factor
        Rectangle dest = {
        paddlePos1.x - (paddleWidth * scaleFactor) / 2,
        paddlePos1.y - (paddleHeight * scaleFactor) / 2,
        paddleWidth * scaleFactor,
        paddleHeight * scaleFactor
    };
    DrawTexturePro(
        paddle1Texture,
        (Rectangle){0, 0, (float)paddle1Texture.width, (float)paddle1Texture.height},
        (Rectangle){paddlePos1.x, paddlePos1.y, paddleWidth, paddleHeight},
        (Vector2){paddleWidth / 2, paddleHeight / 2},
        paddleOrbitAngle1 + 90,
        WHITE
    );
} else {
    // Fallback to original rectangle if texture fails to load
    DrawRectanglePro(
        (Rectangle){paddlePos1.x, paddlePos1.y, paddleWidth, paddleHeight},
        (Vector2){paddleWidth / 2, paddleHeight / 2},
        paddleOrbitAngle1 + 90,
        RED
    );
}

// Paddle2
if (paddle2Texture.id > 0) {
        float scaleFactor = 20.0f; 
        Rectangle dest = {
        paddlePos2.x - (paddleWidth * scaleFactor) / 2,
        paddlePos2.y - (paddleHeight * scaleFactor) / 2,
        paddleWidth * scaleFactor,
        paddleHeight * scaleFactor
    };
    DrawTexturePro(
        paddle2Texture,
        (Rectangle){0, 0, (float)paddle2Texture.width, (float)paddle2Texture.height},
        (Rectangle){paddlePos2.x, paddlePos2.y, paddleWidth, paddleHeight},
        (Vector2){paddleWidth / 2, paddleHeight / 2},
        paddleOrbitAngle2 + 90,
        WHITE
    );
} else {
    // Fallback to original rectangle if texture fails to load
    DrawRectanglePro(
        (Rectangle){paddlePos2.x, paddlePos2.y, paddleWidth, paddleHeight},
        (Vector2){paddleWidth / 2, paddleHeight / 2},
        paddleOrbitAngle2 + 90,
        BLUE
    );
}
            
            EndDrawing();
    } else if (gameState == GAME_OVER) {
    // Game Over Screen
    BeginDrawing();
    DrawTexturePro(
        winScreenTexture,
        (Rectangle){0, 0, (float)winScreenTexture.width, (float)winScreenTexture.height},
        (Rectangle){0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT},
        (Vector2){0, 0},
        0,
        WHITE
    );
    // Determine winner
    const char* winnerText = thrower1HP <= 0 ? "Alpha301 Wins!" : "Centauri903 Wins!";
    DrawText(winnerText, SCREEN_WIDTH/2 - MeasureText(winnerText, 40)/2, SCREEN_HEIGHT/2 - 100, 40, WHITE);

    // Rematch Button
    Rectangle rematchButton = {
        SCREEN_WIDTH/2 - 100, 
        SCREEN_HEIGHT/2, 
        200, 
        100
    };
    DrawTexturePro(
        rematchBtnTexture,
        (Rectangle){0, 0, (float)rematchBtnTexture.width, (float)rematchBtnTexture.height},
        rematchButton,
        (Vector2){0, 0},
        0,
        WHITE
    );

    const char* rematchText = "Rematch?!";
    int rematchFontSize = 20;
    int rematchTextWidth = MeasureText(rematchText, rematchFontSize);
    int rematchTextX = (int)(rematchButton.x + (rematchButton.width / 2) - (rematchTextWidth / 2));
    int rematchTextY = (int)(rematchButton.y + (rematchButton.height / 2) - (rematchFontSize / 2));
    DrawText(rematchText, rematchTextX, rematchTextY, rematchFontSize, MAGENTA);


    // Exit Button
    Rectangle exitButton = {
        SCREEN_WIDTH/2 - 100, 
        rematchButton.y + 70, 
        200, 
        100
    };
    DrawTexturePro(
        exitBtnTexture,
        (Rectangle){0, 0, (float)rematchBtnTexture.width, (float)rematchBtnTexture.height},
        exitButton,
        (Vector2){0, 0},
        0,
        WHITE
    );

    const char* exitText = "Chicken";
    int exitFontSize = 20;
    int exitTextWidth = MeasureText(exitText, exitFontSize);
    int exitTextX = (int)(exitButton.x + (exitButton.width / 2) - (exitTextWidth / 2));
    int exitTextY = (int)(exitButton.y + (exitButton.height / 2) - (exitFontSize / 2));
    DrawText(exitText, exitTextX, exitTextY, exitFontSize, ORANGE);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePoint = GetMousePosition();

                if (CheckCollisionPointRec(mousePoint, rematchButton)) {
                    // Reset game state for rematch
                    gameState = PLAYING;
                    thrower1HP = 5;
                    thrower2HP = 5;
                    isRedTurn = true;
                    currentThrower = &thrower1;
                    ballInAir = false;
                } else if (CheckCollisionPointRec(mousePoint, exitButton)) {
                    CloseWindow(); // Close the game
                }
            }

            EndDrawing();
        }
    }

    CloseWindow(); // Properly close the window on exit
    UnloadTexture(backgroundTexture);
    UnloadTexture(paddle1Texture);
    UnloadTexture(paddle2Texture);
    UnloadTexture(winScreenTexture);
    UnloadTexture(rematchBtnTexture);
    UnloadTexture(exitBtnTexture);

    return 0;
}