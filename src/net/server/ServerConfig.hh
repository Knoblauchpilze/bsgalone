
#pragma once

#include "Connection.hh"
#include <functional>
#include <optional>

namespace net {

using ConnectionAcceptor     = std::function<bool(const Connection &)>;
using ConnectionReadyHandler = std::function<void(ConnectionShPtr)>;

struct ServerConfig
{
  std::optional<ConnectionAcceptor> acceptor{};
  std::optional<DisconnectHandler> disconnectHandler{};
  std::optional<ConnectionReadyHandler> connectionReadyHandler{};
  std::optional<DataReceivedHandler> connectionDataHandler{};
};

} // namespace net
