#include "Phys.h"
#include <cmath>
#include <raylib.h>

Physics::Physics(Vector2 initialPos, float force, float angle, float gravity, float wind)
    : position(initialPos), initialPosition(initialPos),
      force(force), angle(angle),
      gravity(gravity * 2.0f), windFriction(wind),
      trajectoryTime(0.0f), timeAtGround(0.0f) {
        velocity = {0.0f, 0.0f}, Color(color);
}

void Physics::setTexture(const std::string& texturePath) {
    if (useTexture) {
        UnloadTexture(texture);
    }
    texture = LoadTexture(texturePath.c_str());
    if (texture.id > 0) {
        useTexture = true;
    }
}

void Physics::unloadTexture() {
    if (useTexture) {
        UnloadTexture(texture);
        useTexture = false;
    }
}

void Physics::draw() const {
    if (useTexture) {
        DrawTexturePro(
            texture,
            (Rectangle){0, 0, (float)texture.width, (float)texture.height},
            (Rectangle){position.x, position.y, 20, 20},  // 20 is ball size
            (Vector2){10, 10},  // Half of ball size for center origin
            0,
            WHITE
        );
    } else {
        DrawCircleV(position, 10, BLACK);  // Fallback to circle
    }
}

void Physics::update(float deltaTime, float screenWidth, float screenHeight) {
    // Skip update if not moving (ready to launch)
    if (velocity.x == 0.0f && velocity.y == 0.0f) return; 
    
    trajectoryTime += deltaTime;
    velocity.y += gravity * deltaTime;  // Gravity effect
    velocity.x += windFriction * deltaTime;  // Wind effect

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    // Horizontal Collision
    if (position.x + 10 >= screenWidth || position.x - 10 <= 0) {
        velocity.x *= -0.8f;
        position.x = (position.x + 10 >= screenWidth) ? screenWidth - 10 : 10;
    }

    // Vertical collision (Ground)
    if (position.y + 10 >= screenHeight) {
        velocity.y *= -0.25f;
        position.y = screenHeight - 10;

        // If ball is near the ground (screenHeight = 710)
        if (std::abs(position.y - 710) < 10 ) {
            timeAtGround += deltaTime;
            if (timeAtGround >= 0.5f) {
                reset();  // Reset when the ball hits the ground
            }
        } else {
            timeAtGround = 0.0f;  // Reset the timer if ball is still bouncing
        }
    } else {
        timeAtGround = 0.0f;
    }

    // Handle top boundary collision (optional)
    if (position.y - 10 <= 0) {
        velocity.y *= -0.8f;
        position.y = 10;
    }
}

void Physics::reset() {
    // Reset all necessary states back to "ready to launch"
    position = initialPosition;
    velocity = {0.0f, 0.0f};
    trajectoryTime = 0.0f;
    timeAtGround = 0.0f;
}

void Physics::launch(float launchForce, float launchAngle) {
    if (velocity.x == 0.0f && velocity.y == 0.0f) {
        float angleInRadians = launchAngle * (static_cast<float>(M_PI) / 180.0f); // Convert to radians
        velocity.x = launchForce * std::cos(angleInRadians);  // Horizontal velocity
        velocity.y = -launchForce * std::sin(angleInRadians); // Vertical velocity
    }
}

bool Physics::isOutOfBounds(float screenWidth, float screenHeight) const {
    return position.x < 0 || position.x > screenWidth || position.y < 0 || position.y > screenHeight;
}
