
#pragma once

#include "SocketShPtr.hh"
#include <string>

namespace test {

struct ConnectedSockets
{
  net::details::SocketShPtr client{};
  net::details::SocketShPtr server{};

  auto readServer(const std::size_t length) const -> std::string;

  auto readClient(const std::size_t length) const -> std::string;
  void drainClient(const std::size_t length) const;

  template<typename T>
  void writeServer(const T &data) const;
};

} // namespace test

#include "ConnectedSockets.hxx"
