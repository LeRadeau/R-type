#pragma once

#include <string>
#include "Client.hpp"
#include <unordered_map>

class ClientManager {
  public:
    bool addClient(const std::string &id, const ClientNetworking &client)
    {
        if (m_clients.find(id) != m_clients.end())
            return false;
        m_clients[id] = client;
        return true;
    }
    void removeClient(const std::string &id)
    {
        m_clients.erase(id);
    }

  private:
    std::unordered_map<std::string, ClientNetworking> m_clients;
};
