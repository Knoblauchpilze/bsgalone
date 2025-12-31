
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
    FAIL() << "Failed to transfer all bytes of " << data << ", only transferred " << transferred;
  }
}

} // namespace test
