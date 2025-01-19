#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include "network/exceptions/NetworkExceptions.hpp"

class Serializer {
  public:
    // Get the size of the type
    template <typename T> static constexpr size_t getSize()
    {
        return sizeof(T);
    }
    // Append data (int, float, etc.)
    template <typename T> static void serialize(std::vector<uint8_t> &buffer, const T &data)
    {
        const uint8_t *rawData = reinterpret_cast<const uint8_t *>(&data);
        buffer.insert(buffer.end(), rawData, rawData + getSize<T>());
    }

    // Append string (variable length)
    static void serialize(std::vector<uint8_t> &buffer, const std::string &str)
    {
        // Serialize string size
        uint32_t size = static_cast<uint32_t>(str.size());
        serialize(buffer, size);

        // Append string characters
        buffer.insert(buffer.end(), str.begin(), str.end());
    }

    // Deserialize data
    template <typename T> static T deserialize(const uint8_t *&data, size_t &remainingSize)
    {
        if (remainingSize < getSize<T>())
            throw Network::IncompletePacketException("deserialize: remaining size of the buffer is too small");

        T value;
        std::memcpy(&value, data, getSize<T>());
        data += getSize<T>();
        remainingSize -= getSize<T>();
        return value;
    }

    // Deserialize string (variable length)
    static std::string deserializeString(const uint8_t *&data, size_t &remainingSize)
    {
        // Read string size
        uint32_t size = deserialize<uint32_t>(data, remainingSize);
        if (remainingSize < size)
            throw Network::IncompletePacketException("deserializeString: remaining size of the buffer is too small");

        // Extract string content
        std::string str(reinterpret_cast<const char *>(data), size);
        data += size;
        remainingSize -= size;
        return str;
    }
};
