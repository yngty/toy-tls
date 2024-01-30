cmake_minimum_required(VERSION 3.20)

project(toy-tls CXX)

option(LIBTOYTLS_BUILD_DEMOS       "Build all of libtoytls own demos." ON)
option(LIBTOYTLS_BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

include(etc/build_type.cmake)
include(etc/cflags.cmake)
# include(etc/BoringSSL.cmake)

set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBTOYTLS_PROJECT_DIR  ${PROJECT_SOURCE_DIR})

add_subdirectory(src)

if (LIBTOYTLS_BUILD_DEMOS)
    add_subdirectory(demo)
endif()

install(DIRECTORY "include/"
    DESTINATION "include")