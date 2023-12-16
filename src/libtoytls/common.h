#pragma once

#include "util/buffer.h"
#include <cstdint>
#include <vector>

enum RecordType : uint8_t {
  RecordTypeChangeCipherSpec = 20,
  RecordTypeAlert = 21,
  RecordTypeHandshake = 22,
  RecordTypeApplicationData = 23,
};

struct ProtocolVersion {
  uint8_t major;
  uint8_t minor;
};

struct TLSPlaintext {
  RecordType type;
  ProtocolVersion version;
  uint16_t length;
  std::vector<Buffer> payload{};
};

class ClientHello {};