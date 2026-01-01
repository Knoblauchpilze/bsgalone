
#include "TcpServerFixture.hh"

namespace test {

void TcpServerFixture::SetUp()
{
  m_server = std::make_shared<TestTcpServer>();
  m_server->start();
}

void TcpServerFixture::TearDown()
{
  m_server->stop();
  m_server.reset();
}

auto TcpServerFixture::connect() -> net::SocketShPtr
{
  return m_server->connect();
}

auto TcpServerFixture::serverSocket(const std::size_t index) -> net::SocketShPtr
{
  return m_server->socket(index);
}

void TcpServerFixture::write(const std::size_t socketIndex, const std::string &data)
{
  const auto socket = this->serverSocket(socketIndex);

  const auto transferred = asio::write(*socket, asio::buffer(data.data(), data.size()));
  if (transferred != data.size())
  {
    throw std::runtime_error("Failed to transfer all bytes of " + data + ", only transferred "
                             + std::to_string(transferred));
  }
}

auto TcpServerFixture::read(const std::size_t socketIndex, const std::size_t contentLength)
  -> std::string
{
  const auto socket = this->serverSocket(socketIndex);

  std::vector<char> actual(contentLength, 0);
  const auto received = asio::read(*socket, asio::buffer(actual.data(), actual.size()));

  if (received != contentLength)
  {
    throw std::runtime_error("Failed to receive all bytes, expected " + std::to_string(contentLength)
                             + ", only received " + std::to_string(received));
  }

  return std::string(actual.begin(), actual.end());
}

} // namespace test
