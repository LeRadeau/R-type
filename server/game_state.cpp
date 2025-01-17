#include <math.h>
#include "Server.hpp"

void Server::updateBullets(float deltaTime)
{
    for (auto it = bullets_.begin(); it != bullets_.end();) {
        it->position.x += it->velocity.x * deltaTime;
        it->position.y += it->velocity.y * deltaTime;

        if (it->position.x < 0 || it->position.x > 1920 || it->position.y < 0 || it->position.y > 1080) {
            it = bullets_.erase(it);
        } else {
            ++it;
        }
    }
}

void Server::enemyShoot(Ennemy &ennemy)
{
    if (ennemy.shootingCooldown > 0) {
        return;
    }
    std::string bulletID = generateBulletID("enemy");
    Bullet newBullet;
    newBullet.id = bulletID;
    newBullet.position = {ennemy.position.x, ennemy.position.y};
    newBullet.velocity = {-500.0f, 0.0f};
    newBullet.shooter = "enemy";
    bullets_.push_back(newBullet);
    ennemy.shootingCooldown = 2.0f;
}

void Server::updateEnnemies(float deltaTime)
{
    auto enemy = ennemies_.begin();
    while (enemy != ennemies_.end()) {
        if (enemy->isAlive) {
            enemy->position.x -= (enemy->velocity.x) * deltaTime;
            enemy->position.y = enemy->startingY + enemy->amplitude * std::sin(enemy->cosinus * enemy->frequency);
            enemy->cosinus += deltaTime;
            enemy->shootingCooldown -= deltaTime;
            if (enemy->shootingCooldown <= 0) {
                enemyShoot(*enemy);
            }
            enemy++;
        } else {
            enemy = ennemies_.erase(enemy);
        }
    }
}

void Server::loadEnnemies()
{
    for (int i = 0; i < MAX_ENNEMIES; ++i) {
        Ennemy newEnnemy;
        newEnnemy.id = "ennemy_" + std::to_string(i);
        newEnnemy.position = {float(1000), float(100 * i)};
        newEnnemy.velocity = {50, 15};
        newEnnemy.health = 100;
        newEnnemy.shootingCooldown = 1.0f;
        newEnnemy.respawnCooldown = 5.0f;
        newEnnemy.isAlive = true;
        newEnnemy.amplitude = 70.0f;
        newEnnemy.frequency = 1.0f;
        newEnnemy.cosinus = 1;
        newEnnemy.startingY = newEnnemy.position.y;
        ennemies_.push_back(newEnnemy);
    }
}

void Server::spawnEnnemies(int count)
{
    static int enemyCounter = 0;
    float lastSpawnX = 1920.0f;
    const float minSpacingX = 150.0f;

    for (int i = 0; i < count; ++i) {
        Ennemy newEnnemy;
        float screenHeight = 1080.0f;
        newEnnemy.position.x = lastSpawnX + minSpacingX;
        newEnnemy.position.y = static_cast<float>(rand() % static_cast<int>(screenHeight - 20)) + 10.0f;
        lastSpawnX = newEnnemy.position.x;
        newEnnemy.id = "ennemy_" + std::to_string(enemyCounter++);
        newEnnemy.velocity = {50, 15};
        newEnnemy.health = 100;
        newEnnemy.shootingCooldown = static_cast<float>((rand() % 3) + 1);
        newEnnemy.respawnCooldown = 5.0f;
        newEnnemy.isAlive = true;
        newEnnemy.amplitude = 70.0f;
        newEnnemy.frequency = 1.0f;
        newEnnemy.cosinus = 1;
        newEnnemy.startingY = newEnnemy.position.y;
        ennemies_.push_back(newEnnemy);
    }
}

static bool checkCircleCollision(float x1, float y1, float r1, float x2, float y2, float r2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float distanceSquared = dx * dx + dy * dy;
    float radiiSum = r1 + r2;
    float radiiDiff = fabs(r1 - r2);

    return distanceSquared <= (radiiSum * radiiSum) && distanceSquared >= (radiiDiff * radiiDiff);
}

void Server::CheckBulletCollisions()
{
    auto bullet = bullets_.begin();
    while (bullet != bullets_.end()) {
        float bulletX = bullet->position.x;
        float bulletY = bullet->position.y;
        float bulletRadius = 5;
        bool incrementTheBullet = true;
        if (bullet->shooter == "enemy") {
            for (auto &client : clients_) {
                float clientX = client.second.position.x;
                float clientY = client.second.position.y;
                float clientRadius = 32;

                if (!client.second.isAlive)
                    continue;
                if (checkCircleCollision(bulletX, bulletY, bulletRadius, clientX, clientY, clientRadius)) {
                    broadcastBulletHit(bullet->id);
                    client.second.health -= 10;
                    if (client.second.health <= 0) {
                        client.second.isAlive = false;
                        broadcastPlayerDeath(client.second.username);
                    }
                    bullet = bullets_.erase(bullet);
                    incrementTheBullet = false;
                    break;
                }
            }
        } else {
            for (auto &enemy : ennemies_) {
                float enemyX = enemy.position.x;
                float enemyY = enemy.position.y;
                float enemyRadius = 32;

                if (!enemy.isAlive)
                    continue;
                if (checkCircleCollision(bulletX, bulletY, bulletRadius, enemyX, enemyY, enemyRadius)) {
                    broadcastBulletHit(bullet->id);
                    enemy.health -= 10;
                    if (enemy.health <= 0) {
                        enemy.isAlive = false;
                        broadcastEnemyDeath(enemy);
                        for (auto &i : clients_) {
                            if (i.second.username == bullet->shooter) {
                                i.second.score += 10;
                                break;
                            }
                        }
                    }
                    bullet = bullets_.erase(bullet);
                    incrementTheBullet = false;
                    break;
                }
            }
        }
        if (incrementTheBullet)
            bullet++;
    }
}
