
#include "SocketConnector.hh"

// TODO: Should be removed.
#include <iostream>

namespace test {

auto SocketConnector::connect(asio::io_context &context, const int port) -> net::SocketShPtr
{
  std::cout << "[connector] creating socket\n";
  asio::ip::tcp::socket socket(context);
  auto out = std::make_shared<asio::ip::tcp::socket>(std::move(socket));

  asio::ip::tcp::resolver resolver(context);
  auto endpoints = resolver.resolve("127.0.0.1", std::to_string(port));

  std::cout << "[connector] creating future\n";
  auto result = m_connected.get_future();

  std::cout << "[connector] async connect\n";
  asio::async_connect(*out,
                      endpoints,
                      std::bind(&SocketConnector::onConnectionEstablished,
                                shared_from_this(),
                                std::placeholders::_1,
                                std::placeholders::_2));

  std::cout << "[connector] waiting for future\n";
  result.get();
  std::cout << "[connector] got future\n";

  return out;
}

auto SocketConnector::create() -> SocketConnectorShPtr
{
  return std::make_shared<SocketConnector>();
}

void SocketConnector::onConnectionEstablished(const std::error_code code,
                                              const asio::ip::tcp::endpoint & /*endpoint*/)
{
  if (code)
  {
    throw std::runtime_error("Received code " + std::to_string(code.value()) + " (message: \""
                             + code.message() + "\") while connecting to socket");
  }
  std::cout << "[connector] established connection\n";
  m_connected.set_value(true);
}
} // namespace test
