#include "asio_wrapper.h"
#include <map>
#include <memory>
#include <mutex>

class Session;

class Server {
public:
  Server(boost::asio::io_context &io_context, uint16_t port);
  ~Server();
  void clearSession(std::string);
  void start();

private:
  void handleAccept(std::shared_ptr<Session>,
                    const boost::system::error_code &error);

private:
  boost::asio::io_context &io_context_;
  uint16_t port_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::map<std::string, std::shared_ptr<Session>> sessions_;
  std::mutex mutex_;
};