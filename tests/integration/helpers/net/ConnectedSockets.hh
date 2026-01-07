
#pragma once

#include "SocketShPtr.hh"
#include <string>

namespace test {

struct ConnectedSockets
{
  net::details::SocketShPtr client{};
  net::details::SocketShPtr server{};

  auto readServer(const std::size_t length) -> std::string;
  void writeServer(const std::string &data);
};

} // namespace test
