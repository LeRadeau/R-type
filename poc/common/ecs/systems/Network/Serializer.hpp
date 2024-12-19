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
    static void serialize(std::string& buffer, const std::string& str) {
        uint32_t size = static_cast<uint32_t>(str.size());
        serialize(buffer, size);
        buffer.append(str.data(), size);
    }

    // Deserialize data
    template <typename T>
    static T deserialize(const char*& data) {
        T value;
        std::memcpy(&value, data, sizeof(T));
        data += sizeof(T);
        return value;
    }

    // Deserialize string (variable length)
    static std::string deserializeString(const char*& data) {
        uint32_t size = deserialize<uint32_t>(data);
        std::string str(data, size);
        data += size;
        return str;
    }

    enum class MessageType : uint8_t {
        CONNECT,    // Client to Server
        CONNECTED,  // Server to Client
        ERROR,      // Client and Server
        ENTITY,     // CLIENT and Server
        WINDOW,
        INPUT,      // Client and Server
        RENDER,     // Client and Server
        POSITION,   // Server to Client
        NEXT,
        END,
        /*
            Exemple : Packet receivde for client -> 3 6 1204 1238
            - Entity avec Id 3
            - Component type POSITION (6)
            - Position X: 1204
            - Position Y: 1238
            So, if the entity and component are not already create, the Client will create it with positions.
            If entity and component are already create, only change attribut of positions.
            Have to create doc To explain all that.
        */
    
    };
};
