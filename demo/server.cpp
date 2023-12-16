#include "server.h"
#include "asio_context_pool.h"
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>

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
  void start() {}
  const std::string &getUUID() { return uuid_; }

private:
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