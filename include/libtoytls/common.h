#pragma once

#include "buffer.h"
#include "parser.h"
#include <cstdint>
#include <vector>

enum TLSVersion : uint16_t {
  TLSVersionTLS10 = 0x0301,
  TLSVersionTLS11 = 0x0302,
  TLSVersionTLS12 = 0x0303,
  TLSVersionTLS13 = 0x0304,
  TLSVersionSSL30 = 0x0300,
};

enum RecordType : uint8_t {
  RecordTypeChangeCipherSpec = 20,
  RecordTypeAlert = 21,
  RecordTypeHandshake = 22,
  RecordTypeApplicationData = 23,
};

enum HandshakeType : uint8_t {
  HandshakeTypeClientHello = 1,
  HandshakeTypeServerHello = 2,
  HandshakeTypeNewSessionTicket = 4,
  HandshakeTypeEndOfEarlyData = 5,
  HandshakeTypeEncryptedExtensions = 8,
  HandshakeTypeCertificate = 11,
  HandshakeTypeCertificateRequest = 13,
  HandshakeTypeCertificateVerify = 15,
  HandshakeTypeFinished = 20,
  HandshakeTypeKeyUpdate = 24,
  HandshakeTypeMessageHash = 254,
};

enum class TLSCipherSuiteType : uint16_t {
    TLS_RESERVED                                  = 0x8a8a,
    TLS_RSA_WITH_RC4_128_SHA                      = 0x0005,
    TLS_RSA_WITH_3DES_EDE_CBC_SHA                 = 0x000a,
    TLS_RSA_WITH_AES_128_CBC_SHA                  = 0x002f,
    TLS_RSA_WITH_AES_256_CBC_SHA                  = 0x0035,
    TLS_RSA_WITH_AES_128_CBC_SHA256               = 0x003c,
    TLS_RSA_WITH_AES_128_GCM_SHA256               = 0x009c,
    TLS_RSA_WITH_AES_256_GCM_SHA384               = 0x009d,
    TLS_ECDHE_ECDSA_WITH_RC4_128_SHA              = 0xc007,
    TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA          = 0xc009,
    TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA          = 0xc00a,
    TLS_ECDHE_RSA_WITH_RC4_128_SHA                = 0xc011,
    TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA           = 0xc012,
    TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA            = 0xc013,
    TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA            = 0xc014,
    TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256       = 0xc023,
    TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256         = 0xc027,
    TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256         = 0xc02f,
    TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256       = 0xc02b,
    TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384         = 0xc030,
    TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384       = 0xc02c,
    TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256   = 0xcca8,
    TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 = 0xcca9,

    TLS_AES_128_GCM_SHA256       = 0x1301,
    TLS_AES_256_GCM_SHA384       = 0x1302,
    TLS_CHACHA20_POLY1305_SHA256 = 0x1303,
    TLS_AES_128_CCM_SHA256 = 0x1304,
    TLS_AES_128_CCM_8_SHA256 = 0x1305,

    TLS_FALLBACK_SCSV = 0x5600,

    TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305   = TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256,
    TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305 = TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256
};

enum class ExtensionType : uint16_t {
  ExtensionTypeServerName = 0,                           /* RFC 6066 */
  ExtensionTypeMaxFragmentLength = 1,                    /* RFC 6066 */
  ExtensionTypeStatusRequest = 5,                        /* RFC 6066 */
  ExtensionTypeSupportedGroups = 10,                     /* RFC 8422, 7919 */
  ExtensionTypeSignatureAlgorithms = 13,                 /* RFC 8446 */
  ExtensionTypeUseSrtp = 14,                             /* RFC 5764 */
  ExtensionTypeHeartbeat = 15,                           /* RFC 6520 */
  ExtensionTypeApplicationLayerProtocolNegotiation = 16, /* RFC 7301 */
  ExtensionTypeSignedCertificateTimestamp = 18,          /* RFC 6962 */
  ExtensionTypeClientCertificateType = 19,               /* RFC 7250 */
  ExtensionTypeServerCertificateType = 20,               /* RFC 7250 */
  ExtensionTypePadding = 21,                             /* RFC 7685 */
  ExtensionTypePreSharedKey = 41,                        /* RFC 8446 */
  ExtensionTypeEarlyData = 42,                           /* RFC 8446 */
  ExtensionTypeSupportedVersions = 43,                   /* RFC 8446 */
  ExtensionTypeCookie = 44,                              /* RFC 8446 */
  ExtensionTypePskKeyExchangeModes = 45,                 /* RFC 8446 */
  ExtensionTypeCertificateAuthorities = 47,              /* RFC 8446 */
  ExtensionTypeOidFilters = 48,                          /* RFC 8446 */
  ExtensionTypePostHandshakeAuth = 49,                   /* RFC 8446 */
  ExtensionTypeSignatureAlgorithmsCert = 50,             /* RFC 8446 */
  ExtensionTypeKeyShare = 51,                            /* RFC 8446 */
  ExtensionTypeMaxExtensionType = 65535
};

struct ProtocolVersion {
  uint8_t major;
  uint8_t minor;
  void parse(Parser &parser);
};
struct TLSPlaintextHeader {
  static constexpr size_t LENGTH = 5;

  RecordType type;
  ProtocolVersion version;
  uint16_t length;

  void parse(Parser &parser);
};
struct TLSPlaintext {
  TLSPlaintextHeader header;
  std::vector<Buffer> payload{};

  void parse(Parser &parser) {
    header.parse(parser);
    parser.all_remaining(payload);
  }
};

struct HandshakeMsg {
  HandshakeType msg_type;
  uint32_t length;
  std::vector<Buffer> payload;

  void parse(Parser &parser);
};

struct TLSExtension {
  static constexpr size_t FIXED_LENGTH = 4;
  ExtensionType extension_type;
  std::uint16_t extension_data_length;
  std::vector<char> extension_data;

  void parse(Parser& parser);
};

struct ClientHello {
  TLSVersion legacy_version;
  std::array<uint8_t, 32> random;
  std::uint8_t session_id_length;
  std::string session_id;
  std::uint16_t cipher_suites_length;
  std::vector<TLSCipherSuiteType> cipher_suites;
  std::uint8_t legacy_compression_methods_length;
  std::vector<uint8_t> legacy_compression_methods;
  std::uint16_t extensions_length;
  std::vector<TLSExtension> extensions;

  void parse(Parser &parser);
};


