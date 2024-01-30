#pragma once

#include "asio_wrapper.h"

using namespace boost;

using asio::ip::tcp;
using asio::ip::udp;
using asio::awaitable;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;
using asio::buffer;
using asio::io_context;
namespace this_coro = asio::this_coro;

using tcp_socket = asio::use_awaitable_t<>::as_default_on_t<tcp::socket>;
using tcp_resolver = asio::use_awaitable_t<>::as_default_on_t<tcp::resolver>;
using tcp_acceptor = asio::use_awaitable_t<>::as_default_on_t<tcp::acceptor>;
using udp_socket = asio::use_awaitable_t<>::as_default_on_t<udp::socket>;
