#include "Packet.hpp"
#include <cstring>

Packet::Packet(const std::string &senderIp, uint16_t port, int player_id, enum Type type, size_t size, void *data)
    : m_senderIp(senderIp), m_port(port), m_player_id(player_id), m_type(type), m_size(size), m_data(nullptr)
{
    m_length = sizeof(m_player_id) + sizeof(m_type) + sizeof(m_size) + m_size;
    m_buffer = (char *) malloc(m_length * sizeof(char));
    if (m_buffer == nullptr) {
        throw std::bad_alloc();
    }
    memcpy(m_buffer, &m_player_id, sizeof(m_player_id));
    memcpy(m_buffer + sizeof(m_player_id), &m_type, sizeof(m_type));
    memcpy(m_buffer + sizeof(m_player_id) + sizeof(m_type), &m_size, sizeof(m_size));
    memcpy(m_buffer + sizeof(m_player_id) + sizeof(m_type) + sizeof(m_size), m_data, m_size);
    if (m_size > 0) {
        void *tmp = malloc(m_size);
        if (tmp == nullptr) {
            throw std::bad_alloc();
        }
        memcpy(tmp, data, m_size);
    }
}

Packet::Packet(const std::string &senderIp, uint16_t port, const char *buffer, size_t bufferSize)
    : m_senderIp(senderIp), m_port(port), m_length(bufferSize), m_player_id(-1), m_data(nullptr)
{
    memcpy(&m_player_id, buffer, sizeof(m_player_id));
    memcpy(&m_type, buffer + sizeof(m_player_id), sizeof(m_type));
    memcpy(&m_size, buffer + sizeof(m_player_id) + sizeof(m_type), sizeof(m_size));
    if (m_size != 0)
        memcpy(&m_data, buffer + sizeof(m_player_id) + sizeof(m_type) + sizeof(m_size), m_size);
    m_buffer = (char *) malloc(m_length);
    memcpy(m_buffer, buffer, bufferSize);
}

Packet::~Packet()
{
    if (m_buffer)
        free(m_buffer);
    if (m_data != nullptr) {
        free(m_data);
    }
}

const std::string &Packet::getSenderIp() const
{
    return m_senderIp;
}

uint16_t Packet::getPort() const
{
    return m_port;
}

const char *Packet::getBuffer() const
{
    return m_buffer;
}
size_t Packet::getLength() const
{
    return m_length;
}
Packet::Type Packet::getType() const
{
    return m_type;
}
const void *Packet::getData() const
{
    return m_data;
}
size_t Packet::getDataSize() const
{
    return m_size;
}
