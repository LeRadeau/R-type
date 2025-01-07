#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ecs.hpp"

struct PositionComponent : public Component {
    sf::Vector2f position;
    PositionComponent(float x = 0, float y = 0) : position(x, y) {}
};

struct VelocityComponent : public Component {
    sf::Vector2f velocity;
    VelocityComponent(float x = 0, float y = 0) : velocity(x, y) {}
};

struct HealthComponent : public Component {
    int health;
    HealthComponent(int health = 100) : health(health) {}
};

struct RenderComponent : public Component {
    sf::CircleShape shape;
    sf::Color color;
    
    RenderComponent(float radius, sf::Color color) : 
        shape(radius), color(color) {
        shape.setFillColor(color);
    }
};

struct NetworkComponent : public Component {
    std::string username;
    NetworkComponent(const std::string& name) : username(name) {}
};

struct InputComponent : public Component {
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
    bool spaceBar = false;
};

struct usernameComponent : public Component {
    std::string username;
    usernameComponent(const std::string& name) : username(name) {}
};

struct BulletId : public Component {
    std::string id;
    BulletId(const std::string& id) : id(id) {}
};

struct EnnemyId : public Component {
    std::string id;
    EnnemyId(const std::string& id) : id(id) {}
};
