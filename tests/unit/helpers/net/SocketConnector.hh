
#pragma once

#include "SocketPtr.hh"
#include <memory>

namespace test {

class SocketConnector;
using SocketConnectorShPtr = std::shared_ptr<SocketConnector>;

class SocketConnector : public std::enable_shared_from_this<SocketConnector>
{
  public:
  SocketConnector()  = default;
  ~SocketConnector() = default;

  auto connect(asio::io_context &context, const int port) -> net::SocketShPtr;

  static auto create() -> SocketConnectorShPtr;

  private:
  std::promise<bool> m_connected{};

  void onConnectionEstablished(const std::error_code &code,
                               const asio::ip::tcp::endpoint & /*endpoint*/);
};

} // namespace test
