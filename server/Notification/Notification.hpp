#pragma once

#include <memory>
#include <vector>
class Notification {
  public:
    virtual ~Notification() = default;
};

class NotificationBatch {
  public:
    void addNotification(std::shared_ptr<Notification> notification)
    {
        m_notifications.push_back(notification);
    }

    const std::vector<std::shared_ptr<Notification>> &getNotifications() const
    {
        return m_notifications;
    }

  private:
    std::vector<std::shared_ptr<Notification>> m_notifications;
};
