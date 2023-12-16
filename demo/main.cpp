#include "server.h"

using namespace boost;

int main() {
  asio::io_context io_context;
  asio::signal_set signals(io_context, SIGINT, SIGTERM);
  signals.async_wait([&io_context, s ]) Server server(io_context, 12344);
  server.start();
  io_context.run();
  return 0;
}