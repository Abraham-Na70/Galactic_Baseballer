#include "Entity.h"
#include <cmath>
#include <vector>

Entity::Entity(Vector2 pos, float width, float height, Color color, EntityType type)
    : position(pos), width(width), height(height), color(color), entityType(type) {}

void Entity::setTexture(const std::string& texturePath, EntityType type) {
    // Unload existing texture for this type if it exists
    if (textures.find(type) != textures.end()) {
        UnloadTexture(textures[type]);
    }

    // Load new texture
    Texture2D newTexture = LoadTexture(texturePath.c_str());
    if (newTexture.id > 0) {
        textures[type] = newTexture;
        useTexture = true;
    }
}

// Method to check if texture is loaded
bool Entity::hasTexture(EntityType type) const {
    return textures.find(type) != textures.end() && useTexture;
}

void Entity::draw() const {
    float radius = std::min(width, height) / 2.0f;
    
    if (hasTexture(entityType)) {
        // Draw texture based on entity type
        DrawTexturePro(
            textures.at(entityType),
            {0, 0, static_cast<float>(textures.at(entityType).width), 
             static_cast<float>(textures.at(entityType).height)},
            {position.x, position.y, width, height},
            {width/2, height/2},
            0,
            WHITE
        );
    } else {
        // Fallback to original drawing method
        DrawCircle(
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            radius,
            color
        );
    }
}

void Entity::unloadTexture() {
    for (auto& [type, texture] : textures) {
        UnloadTexture(texture);
    }
    textures.clear();
    useTexture = false;
}

Entity::~Entity() {
    unloadTexture();
}

void Entity::drawPolygon(int sides, float radius) const {
    if (sides < 3) return;

    if (hasTexture(entityType)) {
        draw();
    } else {
        std::vector<Vector2> points(sides);
        for (int i = 0; i < sides; ++i) {
            float angle = i * 2.0f * PI / sides;
            points[i] = { 
                position.x + radius * static_cast<float>(cos(angle)),
                position.y + radius * static_cast<float>(sin(angle))
            };
        }

        for (int i = 0; i < sides; ++i) {
            DrawLineEx(points[i], points[(i + 1) % sides], 2.0f, color);
        }
    }
}

void Entity::drawTriangle() const {
    if (hasTexture(EntityType::TRIANGLE)) {
        // Draw texture based on entity type
        DrawTexturePro(
            textures.at(EntityType::TRIANGLE),
            {0, 0, static_cast<float>(textures.at(EntityType::TRIANGLE).width), 
             static_cast<float>(textures.at(EntityType::TRIANGLE).height)},
            {position.x, position.y, width, height},
            {width/2, height/2},
            0,
            WHITE
        );
    } else {
        float radius = std::sqrt(100 * PI / (std::sqrt(3) / 4));
        drawPolygon(3, radius);
    }
}

void Entity::drawRectangle() const {
    if (hasTexture(EntityType::RECTANGLE)) {
        // Draw texture based on entity type
        DrawTexturePro(
            textures.at(EntityType::RECTANGLE),
            {0, 0, static_cast<float>(textures.at(EntityType::RECTANGLE).width), 
             static_cast<float>(textures.at(EntityType::RECTANGLE).height)},
            {position.x, position.y, width, height},
            {width/2, height/2},
            0,
            WHITE
        );
    } else {
        float side = std::sqrt(100 * PI);
        DrawRectangle(
            static_cast<int>(position.x - side / 2),
            static_cast<int>(position.y - side / 2),
            static_cast<int>(side),
            static_cast<int>(side),
            color
        );
    }
}

void Entity::drawPentagon() const {
    if (hasTexture(EntityType::PENTAGON)) {
        // Draw texture based on entity type
        DrawTexturePro(
            textures.at(EntityType::PENTAGON),
            {0, 0, static_cast<float>(textures.at(EntityType::PENTAGON).width), 
             static_cast<float>(textures.at(EntityType::PENTAGON).height)},
            {position.x, position.y, width, height},
            {width/2, height/2},
            0,
            WHITE
        );
    } else {
        float radius = std::sqrt(100 * PI / (5 * std::tan(PI / 5) / 4));
        drawPolygon(5, radius);
    }
}

void Entity::drawHexagon() const {
    if (hasTexture(EntityType::HEXAGON)) {
        // Draw texture based on entity type
        DrawTexturePro(
            textures.at(EntityType::HEXAGON),
            {0, 0, static_cast<float>(textures.at(EntityType::HEXAGON).width), 
             static_cast<float>(textures.at(EntityType::HEXAGON).height)},
            {position.x, position.y, width, height},
            {width/2, height/2},
            0,
            WHITE
        );
    } else {
        float radius = std::sqrt(100 * PI / (3 * std::sqrt(3) / 2));
        drawPolygon(6, radius);
    }
}

// The following functions remain unchanged as they don't deal with textures
void Entity::throwObject(Physics& object, float force, float angle, float multiplier) {
    object = Physics(
        position, 
        force * multiplier, 
        angle, 
        9.8f,
        0.0f
    );
}

bool Entity::checkCollision(const Physics& object) const {
    float radius = std::min(width, height) / 2.0f;
    float distance = std::sqrt(std::pow(object.getPosition().x - position.x, 2) + 
                             std::pow(object.getPosition().y - position.y, 2));
    return distance < (radius + 10);
}

void Entity::setPosition(Vector2 newPos) {
    position = newPos;
}

bool Entity::checkPaddleCollision(const Vector2& paddlePos, float paddleWidth, float paddleHeight, float paddleAngle, Physics& ball) const {
    float angleRad = paddleAngle * DEG2RAD;

    Vector2 relativeBallPos = {
        std::cos(-angleRad) * (ball.getPosition().x - paddlePos.x) - 
        std::sin(-angleRad) * (ball.getPosition().y - paddlePos.y),
        std::sin(-angleRad) * (ball.getPosition().x - paddlePos.x) + 
        std::cos(-angleRad) * (ball.getPosition().y - paddlePos.y)
    };

    bool collisionX = (std::abs(relativeBallPos.x) <= paddleWidth / 2.0f + 10);
    bool collisionY = (std::abs(relativeBallPos.y) <= paddleHeight / 2.0f + 10);

    if (collisionX && collisionY) {
        Vector2 velocity = ball.getVelocity();
        ball.velocity = {
            -velocity.x * 1.65f,
            -velocity.y * 1.65f
        };
        return true;
    }

    return false;
}