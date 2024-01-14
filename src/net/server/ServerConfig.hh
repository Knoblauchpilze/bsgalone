
#pragma once

#include "Connection.hh"
#include <functional>
#include <optional>

namespace net {

using ConnectionAcceptor    = std::function<bool(const Connection &connection)>;
using ConnectionLostHandler = std::function<void(const Connection &connection)>;

struct ServerConfig
{
  std::optional<ConnectionAcceptor> acceptor{};
  std::optional<ConnectionLostHandler> disconnectHandler{};
  std::optional<DataReceivedHandler> connectionDataHandler{};
};

} // namespace net
