#include "ecs.hpp"

// Je ne sais pas où placer cette fonction. Est-ce un composant ? Si, oui : Je pense qu'il faut le mettre dans les RENDERS COMPONENTS
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

    // sf::Texture playerTexture;
    // if (!playerTexture.loadFromFile("../test.png")) {
    //     std::cerr << "Failed to load player texture\n";
    //     return -1;
    // }
    sf::Texture enemytext;
    if (!enemytext.loadFromFile("../test.png")) {
        std::cerr << "Failed to load player texture\n";
        return -1;
    }

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("./GameTests/Rtype/assets/r-typesheet1.gif")) {
        std::cerr << "Failed to load player texture\n";
        return -1;
    }

    Entity player = entityManager.createEntity();
    entityManager.addComponent<PositionComponent>(player, "position", 100.0f, 100.0f);
    entityManager.addComponent<AnimationComponent>(player, "animation", playerTexture, 64, 64, 4, 0.2f); // 4 frames de 64x64 pixels, change toutes les 0.2s
    entityManager.addComponent<VelocityComponent>(player, "velocity", 10.0f, 0.0f, 600);
    entityManager.addComponent<RenderableComponent>(player, "renderable", playerTexture, 0.7);
    entityManager.addComponent<BoundingBoxComponent>(player, "boundingBox", 50.0f, 50.0f);
    entityManager.addComponent<CollisionComponent>(player, "collision");
    entityManager.addComponent<HealthComponent>(player, "health", 100);
    entityManager.addComponent<TimeCooldownComponent>(player, "cooldown", 5.0f);
    entityManager.addComponent<InputComponent>(player, "input");


    Entity enemy2 = entityManager.createEntity();
    entityManager.addComponent<PositionComponent>(enemy2, "position", 500.0f, 100.0f);
    entityManager.addComponent<VelocityComponent>(enemy2, "velocity", 10.0f, 0.0f, 600);
    entityManager.addComponent<RenderableComponent>(enemy2, "renderable", enemytext, 0.5);
    entityManager.addComponent<BoundingBoxComponent>(enemy2, "boundingBox", 50.0f, 50.0f);
    entityManager.addComponent<CollisionComponent>(enemy2, "collision");
    entityManager.addComponent<HealthComponent>(enemy2, "health", 100);
    entityManager.addComponent<DestinationComponent>(enemy2, "destination", 500.0f, 100.0f, 1000.0f, 1000.0f);

    Entity enemy3 = entityManager.createEntity();
    entityManager.addComponent<PositionComponent>(enemy3, "position", 700.0f, 100.0f);
    entityManager.addComponent<VelocityComponent>(enemy3, "velocity", 10.0f, 0.0f, 1800);
    entityManager.addComponent<RenderableComponent>(enemy3, "renderable", enemytext, 0.5);
    entityManager.addComponent<BoundingBoxComponent>(enemy3, "boundingBox", 50.0f, 50.0f);
    entityManager.addComponent<CollisionComponent>(enemy3, "collision");
    entityManager.addComponent<HealthComponent>(enemy3, "health", 100);
    entityManager.addComponent<DestinationComponent>(enemy3, "destination", 700.0f, 100.0f, 0.0f, 800.0f);
    
    for (int i = 9; i > 0; i--) {
        createParallaxLayer(entityManager, "../parallax/city 1/" + std::to_string(i) + ".png", (i * 1.2) * 50.0f, 1.0f);
    }

    InputSystem inputSystem(window);
    MovementSystem movementSystem;
    CollisionSystem collisionSystem;
    RenderSystem renderSystem(window);
    ParallaxSystem parallaxSystem;
    AnimationSystem animationSystem;

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        window.clear(sf::Color::Black);
        inputSystem.processInput(entityManager);
        movementSystem.update(entityManager, dt);
        parallaxSystem.update(entityManager, dt);
        animationSystem.update(entityManager, dt);
        renderSystem.render(entityManager);
        collisionSystem.update(entityManager, dt);
        window.display();
    }
    return 0;
}
