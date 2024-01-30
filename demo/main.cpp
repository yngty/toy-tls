#include "server.h"
#include <iostream>
using namespace boost;

#define ARRAYSIZE(array) \
  (sizeof(::internal::arraySizeHelper(array)))
namespace internal
{
  template<typename T, size_t N>
  auto arraySizeHelper(const T(&array)[N]) -> char(&)[N];
} // namespace internal

int main() {
  asio::io_context io_context;
  asio::signal_set signals(io_context, SIGINT, SIGTERM);
  signals.async_wait([&io_context](auto, auto) {
    io_context.stop();
  }); 
  Server server(io_context, 12344);
  server.start();
  io_context.run();

  constexpr uint32_t a1[] = {
    1,
    2,
    3,
    4,
  };

  constexpr size_t num_a = ARRAYSIZE(a1);
  return 0;
}