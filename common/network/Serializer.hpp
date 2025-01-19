#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

class Serializer {
  public:
    // Append data (int, float, etc.)
    template <typename T> static void serialize(std::vector<uint8_t> &buffer, const T &data)
    {
        const uint8_t *rawData = reinterpret_cast<const uint8_t *>(&data);
        buffer.insert(buffer.end(), rawData, rawData + sizeof(T));
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
    template <typename T> static T deserialize(const uint8_t *&data)
    {
        T value;
        std::memcpy(&value, data, sizeof(T));
        data += sizeof(T);
        return value;
    }

    // Deserialize string (variable length)
    static std::string deserializeString(const uint8_t *&data)
    {
        // Read string size
        uint32_t size = deserialize<uint32_t>(data);

        // Extract string content
        std::string str(reinterpret_cast<const char *>(data), size);
        data += size;

        return str;
    }
};
