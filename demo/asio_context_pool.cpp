#include "asio_context_pool.h"
#include <iostream>
#include <memory>
#include <utility>

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
AsioIOContextPool::AsioIOContextPool(std::size_t size)
    : io_contexts_(size), works_(size), next_io_context_(0) {
  for (std::size_t i = 0; i < size; ++i) {
    works_[i] = std::make_unique<Work>(io_contexts_[i]);
  }

  for (std::size_t i = 0; i < size; ++i) {
    threads_.emplace_back([this, i] { io_contexts_[i].run(); });
  }
#pragma GCC diagnostic pop
}

AsioIOContextPool::~AsioIOContextPool() {
  std::cout << "~AsioIOContextPool()\n";
}

AsioIOContextPool::IOContext &AsioIOContextPool::getIOContext() {
  auto &context = io_contexts_[next_io_context_];
  if (next_io_context_ == io_contexts_.size()) {
    next_io_context_ = 0;
  }
  return context;
}

void AsioIOContextPool::stop() {
  for (auto &work : works_) {
    work.reset();
  }

  for (auto &t : threads_) {
    t.join();
  }
}

AsioIOContextPool &AsioIOContextPool::getInstance() {
  static AsioIOContextPool instance(1);
  return instance;
}