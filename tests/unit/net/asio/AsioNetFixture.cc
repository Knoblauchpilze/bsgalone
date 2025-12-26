
#include "AsioNetFixture.hh"

namespace test {

void AsioNetFixture::SetUp()
{
  m_server = TestTcpServer::create();
}

void AsioNetFixture::TearDown()
{
  m_server.reset();
}

auto AsioNetFixture::connect() -> net::SocketShPtr
{
  return m_server->connect();
}

auto AsioNetFixture::socket(const std::size_t index) -> net::SocketShPtr
{
  return m_server->socket(index);
}

} // namespace test
