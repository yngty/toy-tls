#pragma once

#include "buffer.h"
#include <deque>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

class Serializer;

class Parser {
  class BufferList {
    uint64_t size_{};
    std::deque<Buffer> buffer_{};
    uint64_t skip_{};

  public:
    BufferList(const std::vector<Buffer> &buffers) {
      for (const auto &x : buffers) {
        append(x);
      }
    }

    uint64_t size() const { return size_; }
    uint64_t serialized_length() const { return size(); }
    bool empty() const { return size_ == 0; }

    std::string_view peek() const {
      if (buffer_.empty()) {
        throw std::runtime_error("peek on empty BufferList");
      }

      return std::string_view{buffer_.front()}.substr(skip_);
    }

    void remove_prefix(uint64_t len) {
      while (len > 0 && !buffer_.empty()) {
        const uint64_t to_pop_now = std::min(len, peek().size());
        skip_ += to_pop_now;
        len -= to_pop_now;
        size_ -= to_pop_now;
        if (skip_ == buffer_.front().size()) {
          buffer_.pop_front();
          skip_ = 0;
        }
      }
    }

    void append(Buffer str) {
      size_ += str.size();
      buffer_.push_back(std::move(str));
    }
  };
  BufferList input_;
  bool error_{};
  void check_size(const size_t size) {
    if (size > input_.size()) {
      error_ = true;
    }
  }
};