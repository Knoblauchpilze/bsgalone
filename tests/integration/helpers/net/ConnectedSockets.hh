
#pragma once

#include "SocketShPtr.hh"
#include <string>

namespace test {

struct ConnectedSockets
{
  net::details::SocketShPtr client{};
  net::details::SocketShPtr server{};

  auto readServer(const std::size_t length) -> std::string;

  auto readClient(const std::size_t length) -> std::string;
  void drainClient(const std::size_t length);

  template<typename T>
  void writeServer(const T &data);
};

} // namespace test

#include "ConnectedSockets.hxx"
