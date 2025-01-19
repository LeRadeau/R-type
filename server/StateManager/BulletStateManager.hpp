#pragma once

#include "Observer.hpp"
#include "State/BulletState.hpp"
#include "StateManager.hpp"
#include <unordered_map>

class BulletStateManager : public StateManager, public Observer {
  public:
    void addBullet(const BulletState &bullet);
    void removeBullet(const std::string &bulletId);
    void update(float deltaTime) override;

    void onNotify(const Notification &notification) override;

    const std::unordered_map<std::string, BulletState> &getAllBullets() const;

  private:
    std::unordered_map<std::string, BulletState> m_bullets;
};
