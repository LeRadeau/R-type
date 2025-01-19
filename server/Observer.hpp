#pragma once

#include <algorithm>
#include <mutex>
#include <vector>
#include "Notification/Notification.hpp"

class Observer {
  public:
    virtual ~Observer() = default;
    virtual void onNotify(const Notification &notification) = 0;
};

class Subject {
  public:
    void addObserver(Observer *observer)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_observers.push_back(observer);
    }

    void removeObserver(Observer *observer)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
    }

  protected:
    void notify(const Notification &notification)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (Observer *observer : m_observers) {
            observer->onNotify(notification);
        }
    }

    void notify(const NotificationBatch &notifications)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (Observer *observer : m_observers) {
            for (auto notification : notifications.getNotifications())
                observer->onNotify(*notification.get());
        }
    }

  private:
    std::vector<Observer *> m_observers;
    std::mutex m_mutex;
};
