#pragma once

#include "asio_wrapper.h"
#include <vector>

class AsioIOContextPool final {
public:
  using IOContext = boost::asio::io_context;
  using Work = IOContext::work;
  using WorkPtr = std::unique_ptr<Work>;
  ~AsioIOContextPool();
  AsioIOContextPool(const AsioIOContextPool &) = delete;
  AsioIOContextPool &operator=(const AsioIOContextPool &) = delete;

  IOContext &getIOContext();
  void stop();
  static AsioIOContextPool &getInstance();

private:
  AsioIOContextPool(std::size_t size = std::thread::hardware_concurrency());
  std::vector<IOContext> io_contexts_;
  std::vector<WorkPtr> works_;
  std::vector<std::thread> threads_;
  std::size_t next_io_context_;
};
