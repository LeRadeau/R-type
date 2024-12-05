#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <cmath>

using Entity = std::size_t;

class Component {
public:
    virtual ~Component() = default;
};

struct PositionComponent : public Component {
    float x, y;
    PositionComponent(float x = 0, float y = 0) : x(x), y(y) {}
};

struct VelocityComponent : public Component {
    float vx, vy, speed;
    VelocityComponent(float vx = 0, float vy = 0, float speed = 0) : vx(vx), vy(vy), speed(speed) {}
};

struct DestinationComponent : public Component {
    float depx, depy;
    float desx, desy;
    bool todes = false;
    DestinationComponent(float depx = 0, float depy = 0, float desx = 0, float desy = 0) : depx(depx), depy(depy), desx(desx), desy(desy) {}
};


struct RenderableComponent : public Component {
    sf::Sprite sprite;
    float scale;

    RenderableComponent(const sf::Texture& texture, float scale = 1.0f) : scale(scale) {
        sprite.setTexture(texture);
        sprite.setScale(scale, scale);
    }
};

struct InputComponent : public Component {
    bool moveUp = false;
    bool moveDown = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool spaceBar = false;
};

struct BackgroundComponent : public Component {
    sf::Sprite sprite;
    float parallaxSpeed;
    float scale;

    BackgroundComponent(const sf::Texture& texture, float parallaxSpeed = 0, float scale = 1.0f) : parallaxSpeed(parallaxSpeed), scale(scale) {
        sprite.setTexture(texture);
        sprite.setScale(scale, scale);
    }
};

class EntityManager {
private:
    std::unordered_map<Entity, std::unordered_map<std::string, std::shared_ptr<Component>>> components;
    Entity nextEntity = 0;

public:
    Entity createEntity() {
        return nextEntity++;
    }

    template<typename T, typename... Args>
    void addComponent(Entity entity, const std::string& name, Args&&... args) {
        components[entity][name] = std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    std::shared_ptr<T> getComponent(Entity entity, const std::string& name) {
        return std::static_pointer_cast<T>(components[entity][name]);
    }

    template<typename T>
    std::vector<Entity> getEntitiesWithComponent(const std::string& name) {
        std::vector<Entity> result;
        for (const auto& [entity, compMap] : components) {
            if (compMap.find(name) != compMap.end()) {
                result.push_back(entity);
            }
        }
        return result;
    }
};

class MovementSystem {
public:
    void update(EntityManager& em, float dt) {
        // Process entities with InputComponent for movement
        auto inputEntities = em.getEntitiesWithComponent<InputComponent>("input");
        for (Entity e : inputEntities) {
            auto position = em.getComponent<PositionComponent>(e, "position");
            auto velocity = em.getComponent<VelocityComponent>(e, "velocity");
            auto input = em.getComponent<InputComponent>(e, "input");

            if (position && velocity) {
                velocity->vx = 0.0f;
                velocity->vy = 0.0f;
                if (input->moveUp)      velocity->vy = -velocity->speed;
                if (input->moveDown)    velocity->vy = velocity->speed;
                if (input->moveLeft)    velocity->vx = -velocity->speed;
                if (input->moveRight)   velocity->vx = velocity->speed;

                if (velocity->vx != 0 && velocity->vy != 0) {
                    velocity->vx *= 0.7071f;
                    velocity->vy *= 0.7071f;
                }
                position->x += velocity->vx * dt;
                position->y += velocity->vy * dt;
            }
        }

        // Process entities with DestinationComponent for movement
        auto destinationEntities = em.getEntitiesWithComponent<DestinationComponent>("destination");
        for (Entity e : destinationEntities) {
            auto position = em.getComponent<PositionComponent>(e, "position");
            auto velocity = em.getComponent<VelocityComponent>(e, "velocity");
            auto destination = em.getComponent<DestinationComponent>(e, "destination");

            if (position && velocity && destination) {
                if (destination->todes) {
                    float dx = destination->desx - position->x;
                    float dy = destination->desy - position->y;
                    float angle = std::atan2(dy, dx);
                    velocity->vx = velocity->speed * std::cos(angle);
                    velocity->vy = velocity->speed * std::sin(angle);

                    if (std::abs(dx) < 5 && std::abs(dy) < 5) {
                        destination->todes = false;
                    }
                }
                else {
                    float dx = destination->depx - position->x;
                    float dy = destination->depy - position->y;
                    float angle = std::atan2(dy, dx);
                    velocity->vx = velocity->speed * std::cos(angle);
                    velocity->vy = velocity->speed * std::sin(angle);

                    if (std::abs(dx) < 10 && std::abs(dy) < 10) {
                        destination->todes = true;
                    }
                }
                position->x += velocity->vx * dt;
                position->y += velocity->vy * dt;
            }
        }
    }
};


class InputSystem {
private:
    sf::RenderWindow& window;

public:
    explicit InputSystem(sf::RenderWindow& window) : window(window) {}

    void processInput(EntityManager& em) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        auto entities = em.getEntitiesWithComponent<InputComponent>("input");
        for (Entity e : entities) {
            auto input = em.getComponent<InputComponent>(e, "input");
            if (input) {
                input->moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
                input->moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
                input->moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
                input->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
                input->spaceBar = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
            }
        }
    }
};

class ParallaxSystem {
public:
    void update(EntityManager& em, float dt) {
        auto entities = em.getEntitiesWithComponent<BackgroundComponent>("background");
        for (Entity e : entities) {
            auto background = em.getComponent<BackgroundComponent>(e, "background");
            auto position = em.getComponent<PositionComponent>(e, "position");
            
            if (background && position) {
                position->x -= background->parallaxSpeed * dt;

                if (position->x < -1920) position->x = 0;
            }
        }
    }
};

class RenderSystem {
private:
    sf::RenderWindow& window;

public:
    explicit RenderSystem(sf::RenderWindow& window) : window(window) {}

    void render(EntityManager& em) {
        // Render background layers first
        auto backgroundEntities = em.getEntitiesWithComponent<BackgroundComponent>("background");
        for (Entity e : backgroundEntities) {
            auto background = em.getComponent<BackgroundComponent>(e, "background");
            auto position = em.getComponent<PositionComponent>(e, "position");

            if (background && position) {
                background->sprite.setPosition(position->x, position->y);
                window.draw(background->sprite);
            }
        }

        auto entities = em.getEntitiesWithComponent<RenderableComponent>("renderable");
        for (Entity e : entities) {
            auto renderable = em.getComponent<RenderableComponent>(e, "renderable");
            auto position = em.getComponent<PositionComponent>(e, "position");

            if (renderable && position) {
                renderable->sprite.setPosition(position->x, position->y);
                window.draw(renderable->sprite);
            }
        }
    }
};


void createParallaxLayer(EntityManager& entityManager, const std::string& texturePath, float parallaxSpeed, float scale) {
    static std::unordered_map<std::string, sf::Texture> textureCache;

    if (textureCache.find(texturePath) == textureCache.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile(texturePath)) {
            std::cerr << "Failed to load texture: " << texturePath << '\n';
            return;
        }
        textureCache[texturePath] = texture;
    }

    Entity entity = entityManager.createEntity();
    entityManager.addComponent<PositionComponent>(entity, "position", 0.0f, 0.0f);
    entityManager.addComponent<BackgroundComponent>(entity, "background", textureCache[texturePath], parallaxSpeed, scale);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "ECS");
    window.setFramerateLimit(120);

    EntityManager entityManager;

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("test.png")) {
        std::cerr << "Failed to load player texture\n";
        return -1;
    }
    sf::Texture enemytext;
    if (!enemytext.loadFromFile("test.png")) {
        std::cerr << "Failed to load player texture\n";
        return -1;
    }

    Entity player = entityManager.createEntity();
    entityManager.addComponent<PositionComponent>(player, "position", 100.0f, 100.0f);
    entityManager.addComponent<VelocityComponent>(player, "velocity", 10.0f, 0.0f, 600);
    entityManager.addComponent<RenderableComponent>(player, "renderable", playerTexture, 0.7);
    entityManager.addComponent<InputComponent>(player, "input");

    Entity enemy = entityManager.createEntity();
    entityManager.addComponent<PositionComponent>(enemy, "position", 300.0f, 100.0f);
    entityManager.addComponent<VelocityComponent>(enemy, "velocity", 10.0f, 0.0f, 400);
    entityManager.addComponent<RenderableComponent>(enemy, "renderable", enemytext, 0.5);
    entityManager.addComponent<DestinationComponent>(enemy, "destination", 300.0f, 100.0f, 500.0f, 500.0f);

    Entity enemy2 = entityManager.createEntity();
    entityManager.addComponent<PositionComponent>(enemy2, "position", 500.0f, 100.0f);
    entityManager.addComponent<VelocityComponent>(enemy2, "velocity", 10.0f, 0.0f, 600);
    entityManager.addComponent<RenderableComponent>(enemy2, "renderable", enemytext, 0.5);
    entityManager.addComponent<DestinationComponent>(enemy2, "destination", 500.0f, 100.0f, 1000.0f, 1000.0f);

    Entity enemy3 = entityManager.createEntity();
    entityManager.addComponent<PositionComponent>(enemy3, "position", 700.0f, 100.0f);
    entityManager.addComponent<VelocityComponent>(enemy3, "velocity", 10.0f, 0.0f, 1800);
    entityManager.addComponent<RenderableComponent>(enemy3, "renderable", enemytext, 0.5);
    entityManager.addComponent<DestinationComponent>(enemy3, "destination", 700.0f, 100.0f, 0.0f, 800.0f);

    
    for (int i = 9; i > 0; i--) {
        createParallaxLayer(entityManager, "parallax/city 1/" + std::to_string(i) + ".png", (i * 1.2) * 50.0f, 1.0f);
    }

    InputSystem inputSystem(window);
    MovementSystem movementSystem;
    RenderSystem renderSystem(window);
    ParallaxSystem parallaxSystem;

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        window.clear(sf::Color::Black);
        inputSystem.processInput(entityManager);
        movementSystem.update(entityManager, dt);
        parallaxSystem.update(entityManager, dt);
        renderSystem.render(entityManager);
        window.display();
    }
    return 0;
}
