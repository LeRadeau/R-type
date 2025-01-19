#pragma once
#include "Notification.hpp"

class PlayerCreationNotification : public Notification {
  public:
    explicit PlayerCreationNotification(const std::string &username) : m_username(username)
    {
    }

    const std::string &getUsername() const
    {
        return m_username;
    }

  private:
    std::string m_username;
};
