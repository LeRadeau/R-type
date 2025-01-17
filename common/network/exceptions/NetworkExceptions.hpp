#pragma once

#include <stdexcept>
namespace Network
{
    class DeserializationException : public std::runtime_error {
      public:
        explicit DeserializationException(const std::string &message) : std::runtime_error(message)
        {
        }
    };

    class InvalidPacketFormatException : public DeserializationException {
      public:
        explicit InvalidPacketFormatException(const std::string &message)
            : DeserializationException("Invalid Packet Format: " + message)
        {
        }
    };

    class IncompletePacketException : public DeserializationException {
      public:
        explicit IncompletePacketException(const std::string &message)
            : DeserializationException("Incomplete Packet: " + message)
        {
        }
    };

    class UnsupportedPacketVersionException : public DeserializationException {
      public:
        explicit UnsupportedPacketVersionException(const std::string &message)
            : DeserializationException("Unsupported Packet Version: " + message)
        {
        }
    };

    class NetworkManagerException : public std::runtime_error {
      public:
        explicit NetworkManagerException(const std::string &message) : std::runtime_error(message)
        {
        }
    };

    class SocketBindingException : public NetworkManagerException {
      public:
        explicit SocketBindingException(const std::string &message)
            : NetworkManagerException("SocketBindingException: " + message)
        {
        }
    };

    class SocketListeningException : public NetworkManagerException {
      public:
        explicit SocketListeningException(const std::string &message)
            : NetworkManagerException("SocketListeningException: " + message)
        {
        }
    };

    class ConnectionException : public NetworkManagerException {
      public:
        explicit ConnectionException(const std::string &message)
            : NetworkManagerException("ConnectionException: " + message)
        {
        }
    };

    class TransmissionException : public NetworkManagerException {
      public:
        explicit TransmissionException(const std::string &message)
            : NetworkManagerException("TransmissionException: " + message)
        {
        }
    };

    class TimeoutException : public NetworkManagerException {
      public:
        explicit TimeoutException(const std::string &message) : NetworkManagerException("TimeoutException: " + message)
        {
        }
    };
} // namespace Network
