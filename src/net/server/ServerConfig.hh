
#pragma once

#include "Connection.hh"
#include <functional>
#include <optional>

namespace net {

using ConnectionAcceptor = std::function<bool(const Connection &connection)>;

struct ServerConfig
{
  std::optional<ConnectionAcceptor> acceptor{};
};

} // namespace net
