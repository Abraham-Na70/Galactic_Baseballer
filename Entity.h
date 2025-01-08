#pragma once
#include <raylib.h>
#include <cmath>
#include <vector>
#include "Phys.h"
#include <string>
#include <unordered_map>

class Entity {
public:
    enum class EntityType {
        DEFAULT,
        TRIANGLE,
        RECTANGLE,
        PENTAGON,
        HEXAGON,
        PADDLE,
        BALL
    };

    using PolygonDrawMethod = void (Entity::*)() const;

    Entity(Vector2 pos = {0, 0}, float width = 20.0f, float height = 40.0f, 
           Color color = RED, EntityType type = EntityType::DEFAULT);

    void draw() const;
    void setTexture(const std::string& texturePath, EntityType type = EntityType::DEFAULT);
    bool hasTexture(EntityType type = EntityType::DEFAULT) const;
    void throwObject(Physics& object, float force, float angle, 
        float multiplier = 1.0f);
    
    Vector2 getPosition() const {return position;} 
    void setPosition(Vector2 newPos);
    bool checkCollision(const Physics& object) const;
    bool checkPaddleCollision(const Vector2& paddlePos, float paddleWidth, float paddleHeight, float paddleAngle, Physics& ball) const;

    void drawTriangle() const;
    void drawRectangle() const;
    void drawPentagon() const;
    void drawHexagon() const;

    float getWidth() const { return width; }
    float getHeight() const { return height; }
    ~Entity();
    PolygonDrawMethod polygonType = nullptr;

    // Fixed these functions - changed EntityType to entityType
    void setEntityType(EntityType type) { entityType = type; }
    EntityType getEntityType() const { return entityType; }

private:
    Vector2 position;
    float height;
    float width;
    std::unordered_map<EntityType, Texture2D> textures;  // Store multiple textures
    EntityType entityType;  // This is the member variable
    bool useTexture = false;  // Flag to indicate if texture should be used
    void drawPolygon(int sides, float radius) const;
    void unloadTexture();  // Add method to unload texture
    Color color;
};