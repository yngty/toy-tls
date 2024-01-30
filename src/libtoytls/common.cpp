#include "libtoytls/common.h"

using namespace std;

void ProtocolVersion::parse(Parser &parser) {
  parser.integer(major);
  parser.integer(minor);
}

void TLSPlaintextHeader::parse(Parser &parser) {
  uint8_t v;
  parser.integer(v);
  type = static_cast<RecordType>(v);
  version.parse(parser);
  parser.integer(length);
}

void HandshakeMsg::parse(Parser &parser) {
  uint8_t type_v;
  parser.integer(type_v);
  msg_type = static_cast<HandshakeType>(type_v);

  uint16_t length_v1{};
  uint8_t length_v2{};
  parser.integer(length_v1);
  parser.integer(length_v2);
  length = (static_cast<uint32_t>(length_v1) << 8) | length_v2;
  string str;
  str.resize(length);
  parser.string(str);
  payload.emplace_back(std::move(str));
}

void TLSExtension::parse(Parser &parser) {
  uint16_t type_v;
  parser.integer(type_v);
  extension_type = static_cast<ExtensionType>(type_v);
  parser.integer(extension_data_length);
  extension_data.resize(extension_data_length);
  parser.string(extension_data);
}

void ClientHello::parse(Parser &parser) {
  uint16_t version;
  parser.integer(version);
  legacy_version = static_cast<TLSVersion>(version);
  for (auto &i : random) {
    parser.integer(i);
  }
  parser.integer(session_id_length);
  session_id.resize(session_id_length);
  parser.string(session_id);

  parser.integer(cipher_suites_length);
  cipher_suites.resize(cipher_suites_length / sizeof(uint16_t));
  for (auto &cs : cipher_suites) {
     uint16_t v;
    parser.integer(v);
     cs = static_cast<TLSCipherSuiteType>(v);
  }

  parser.integer(legacy_compression_methods_length);
  legacy_compression_methods.resize(legacy_compression_methods_length);
  for (auto &cm : legacy_compression_methods) {
    parser.integer(cm);
  }

  parser.integer(extensions_length);
  uint16_t  remaining_size = extensions_length;

  while (remaining_size > 0) {
    TLSExtension extension;
    extension.parse(parser);
    if (parser.has_error())
      return;
    remaining_size -= (extension.extension_data_length + TLSExtension::FIXED_LENGTH);
    extensions.push_back(std::move(extension));
  }
}