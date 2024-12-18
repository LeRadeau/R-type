#include "Serializer.hpp"

void Serializer::serialize(std::string &buffer, const std::string &str) {
  uint32_t size = static_cast<uint32_t>(str.size());
  serialize(buffer, size);
  buffer.append(str.data(), size);
}

std::string Serializer::deserializeString(const char *&data) {
  uint32_t size = deserialize<uint32_t>(data);
  std::string str(data, size);
  data += size;
  return str;
}
