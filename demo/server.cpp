#include "server.h"
#include "asio_context_pool.h"
#include "awaitable_default.h"
#include "libtoytls/common.h"
#include "libtoytls/parser.h"
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <ranges>

using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
  Session(asio::io_context &io_context, Server *server)
      : io_context_(io_context), server_(server), socket_(io_context), uuid_{} {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    uuid_ = boost::uuids::to_string(uuid);
  }
  Session(const Session &) = delete;
  Session &operator=(const Session &) = delete;
  tcp::socket &getSocket() { return socket_; }
  void start() {
    auto shred_this = shared_from_this();
    co_spawn(
        io_context_,
        [=]() -> awaitable<void> {
          try {
            vector<string> strs(2);
            strs.front().resize(TLSPlaintextHeader::LENGTH);
            co_await read(strs);

            TLSPlaintext plaintext;
            vector<Buffer> buffers;
            ranges::transform(strs, back_inserter(buffers), identity());
            if (parse(plaintext, buffers)) {
              std::cout << plaintext.header.type << std::endl;
              HandshakeMsg handshake_msg;
              if (parse(handshake_msg, plaintext.payload)) {
                switch (handshake_msg.msg_type) {
                case HandshakeTypeClientHello: {
                  ClientHello hello_msg;
                  if (parse(hello_msg, handshake_msg.payload)) {
                    std::cout << handshake_msg.msg_type << std::endl;
                  }
                }
                break;
                default:
                  break;
                }
              }
            }
          } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
          }
        },
        detached);
  }
  const std::string &getUUID() { return uuid_; }

private:
  awaitable<void> read(vector<std::string> &strs) {
    try {
      if (strs.empty()) {
        co_return;
      }
      strs.back().clear();
      strs.back().resize(kReadBufferSize);

      std::vector<asio::mutable_buffer> buffers;
      size_t total_size = 0;
      for (const auto &str : strs) {
        buffers.emplace_back(const_cast<char *>(str.data()), str.size());
        total_size += str.size();
      }

      size_t bytes_read =
          co_await socket_.async_read_some(buffers, use_awaitable);
      if (bytes_read > static_cast<ssize_t>(total_size)) {
        throw runtime_error("max than page");
      }

      std::cout << "recev " << bytes_read << " bytes\n";
      size_t remaining_size = bytes_read;
      for (auto &str : strs) {
        if (remaining_size >= str.size()) {
          remaining_size -= str.size();
        } else {
          str.resize(remaining_size);
          remaining_size = 0;
        }
      }
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
  }

private:
  static constexpr size_t kReadBufferSize = 16384;
  asio::io_context &io_context_;
  Server *server_;
  tcp::socket socket_;
  std::string uuid_;
};
Server::Server(asio::io_context &io_context, uint16_t port)
    : io_context_(io_context), port_(port),
      acceptor_(tcp::acceptor(io_context, tcp::endpoint(tcp::v4(), port))),
      sessions_() {}

Server::~Server() {}

void Server::handleAccept(std::shared_ptr<Session> session,
                          const boost::system::error_code &error) {
  if (!error) {
    session->start();
    std::unique_lock lk(mutex_);
    sessions_.emplace(std::move(session->getUUID()), session);
  } else {
    std::cerr << "session accept failed error is: " << error.message()
              << std::endl;
  }
  start();
}
void Server::start() {
  auto &io_context = AsioIOContextPool::getInstance().getIOContext();
  auto session = std::make_shared<Session>(io_context, this);
  acceptor_.async_accept(
      session->getSocket(),
      std::bind(&Server::handleAccept, this, session, std::placeholders::_1));
}

void Server::clearSession(std::string uuid) {
  std::unique_lock lk(mutex_);
  sessions_.erase(std::move(uuid));
}