#pragma once
#include <raylib.h>
#include <cmath>
#include <string>

class Physics {
public:
    Physics(Vector2 initialPos = {0, 0}, 
            float force = 0, 
            float angle = 0, 
            float gravity = 9.8f, 
            float wind = 0);

    void update(float deltaTime, float screenWidth, float screenHeight);
    void draw() const;
    void launch(float launchForce, float launchAngle);
    void reset();
    bool isOutOfBounds(float screenWidth, float screenHeight) const;
    void applyWind(float windForce);
    void setTexture(const std::string& texturePath);
    void unloadTexture();

    Vector2 getPosition() const { return position; }
    Vector2 getVelocity() const { return velocity; }
    Vector2 position;
    Vector2 velocity{0, 0};
    Vector2 initialPosition;
    void resetTrajectoryTime() { trajectoryTime = 0.0f; }
    Color color;

    bool isReadyToLaunch() const { 
        return velocity.x == 0.0f && velocity.y == 0.0f; 
    }

private:
    float force;
    float angle;
    float gravity;
    float windFriction;
    float trajectoryTime;
    float timeAtGround;
    Texture2D texture;
    bool useTexture = false;
};