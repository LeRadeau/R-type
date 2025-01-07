#pragma once
#include <string>
#include <cstring>
#include <cstdint>

class Serializer {
public:
    // Append data (int, float)
    template <typename T>
    static void serialize(std::string& buffer, const T& data) {
        const char* rawData = reinterpret_cast<const char*>(&data);
        buffer.append(rawData, sizeof(T));
    }

    // Append string (variable length)
    static void serialize(std::string& buffer, const std::string& str); 

    // Deserialize data
    template <typename T>
    static T deserialize(const char*& data) {
        T value;
        std::memcpy(&value, data, sizeof(T));
        data += sizeof(T);
        return value;
    }

    // Deserialize string (variable length)
    static std::string deserializeString(const char*& data);
};
