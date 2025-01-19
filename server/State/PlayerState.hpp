#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <optional>

struct PlayerStateUpdate {
    std::optional<sf::Vector2f> position;
    std::optional<int> health;
    std::optional<int> score;
    std::optional<bool> isAlive;

    PlayerStateUpdate(std::optional<sf::Vector2f> position = std::nullopt, std::optional<int> health = std::nullopt,
        std::optional<int> score = std::nullopt, std::optional<bool> alive = std::nullopt)
        : position(position), health(health), score(score), isAlive(alive)
    {
    }
};

class PlayerState {
  public:
    PlayerState(const std::string &name);
    PlayerState(const std::string &name, const PlayerStateUpdate &update);
    const std::string &getUsername() const;
    const sf::Vector2f &getPosition() const;
    int getHealth() const;
    int getScore() const;
    bool getIsAlive() const;

    void applyUpdate(const PlayerStateUpdate &update);

  private:
    std::string m_username;
    sf::Vector2f m_position;
    int m_health;
    int m_score;
    bool m_isAlive;
};
