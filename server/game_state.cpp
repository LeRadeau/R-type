#include "Server.hpp"
#include <math.h> 
#include <iostream>

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
    for (auto &ennemy : ennemies_) {
        if (ennemy.isAlive) {
            ennemy.position.x += (ennemy.velocity.x) * deltaTime;

            ennemy.position.y = ennemy.startingY + ennemy.amplitude * std::sin(ennemy.cosinus * ennemy.frequency);
            ennemy.cosinus += deltaTime;
            if (ennemy.shootingCooldown <= 0) {
                enemyShoot(ennemy);
            }
        } else {
            ennemy.respawnCooldown -= deltaTime;
            if (ennemy.respawnCooldown <= 0) {
                ennemy.isAlive = true;
                ennemy.health = 100;
            }
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

void Server::CheckEnnemyCollision()
{
    for (auto &ennemy : ennemies_) {
        for (auto &bullet : bullets_) {
            if (bullet.shooter == "enemy") {
                continue;
            }
            if (ennemy.isAlive && 
                bullet.position.x > ennemy.position.x - 20 &&
                bullet.position.x < ennemy.position.x + 20 &&
                bullet.position.y > ennemy.position.y - 20 &&
                bullet.position.y < ennemy.position.y + 20) {

                ennemy.health -= 10;
                if (ennemy.health <= 0) {
                    ennemy.isAlive = false;
                    ennemy.respawnCooldown = 5.0f;
                }
                bullet.position = {2000, 2000};
            }
        }
    }
}
