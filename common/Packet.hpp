#pragma once

#include <cstdint>
#include <string>

class Packet {
  public:
    enum class Type {
        PLAYER_JOIN,
        PLAYER_LEAVE,
        PLAYER_ID,
        PLAYER_MOVE,
        PLAYER_SHOOT,
        PLAYER_CHARGE,
        PLAYER_POSITION,
        PLAYER_DEATH,
        NPC_SHOOT,
        NPC_DEATH,
        PROJECTILE_HIT,
        GAME_START,
    };

  private:
    std::string m_senderIp;
    uint16_t m_port;
    size_t m_length;
    int m_player_id;
    enum Type m_type;
    size_t m_size;
    void *m_data;
    char *m_buffer;

  public:
    Packet(const std::string &senderIp, uint16_t port, int player_id, enum Type type, size_t size, void *data);
    Packet(const std::string &senderIp, uint16_t port, const char *buffer, size_t bufferSize);
    ~Packet();
    const std::string &getSenderIp() const;
    uint16_t getPort() const;
    const char *getBuffer() const;
    size_t getLength() const;
    int getPlayerId() const;
    Packet::Type getType() const;
    const void *getData() const;
    size_t getDataSize() const;
};
