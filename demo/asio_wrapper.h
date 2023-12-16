#pragma once

#include <utility>

#define GNU_CC_VERSION                                                         \
  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if (defined(__GNUC__) &&                                                      \
     ((GNU_CC_VERSION >= 50200) && (GNU_CC_VERSION < 60000)) &&                \
     defined(BOOST_ERROR_CODE_HEADER_ONLY))
#pragma GCC push_options
#pragma GCC optimize("O0")
#include <boost/asio.hpp>
#pragma GCC pop_options
#else
#include <boost/asio.hpp>
#endif
