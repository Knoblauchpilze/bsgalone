
#include "TcpAcceptor.hh"

namespace test {

TcpAcceptor::TcpAcceptor(asio::io_context &context, const int port)
  : m_acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{}

void TcpAcceptor::registerAccept()
{
  m_acceptor.async_accept(std::bind(&TcpAcceptor::onConnectionRequest,
                                    shared_from_this(),
                                    std::placeholders::_1,
                                    std::placeholders::_2));
}

auto TcpAcceptor::waitForServerSocket() -> net::SocketShPtr
{
  std::unique_lock guard(m_locker);

  // This timeout should be enough for most test scenario. If it becomes
  // an issue it can be made configurable.
  constexpr auto REASONABLE_TIMEOUT = std::chrono::seconds(5);
  m_notifier.wait_for(guard, REASONABLE_TIMEOUT, [this] { return m_socket != nullptr; });

  net::SocketShPtr out{};
  std::swap(out, m_socket);

  if (!out)
  {
    throw std::runtime_error("Failed to connect to server, got null socket");
  }

  return out;
}

void TcpAcceptor::onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket)
{
  if (code)
  {
    throw std::runtime_error("Received code " + std::to_string(code.value()) + " (message: \""
                             + code.message() + "\") while processing incoming connection");
  }

  registerAccept();

  std::unique_lock guard(m_locker);
  m_socket = std::make_shared<asio::ip::tcp::socket>(std::move(socket));
  m_notifier.notify_one();
}

} // namespace test
