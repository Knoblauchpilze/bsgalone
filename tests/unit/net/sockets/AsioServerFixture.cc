
#include "AsioServerFixture.hh"
#include "DataSender.hh"
#include <chrono>

namespace test {

void AsioServerFixture::SetUp()
{
  m_server = TestTcpServer::create();
}

void AsioServerFixture::TearDown()
{
  m_server->stop();
  m_server.reset();
}

auto AsioServerFixture::connect() -> net::SocketShPtr
{
  return m_server->connect();
}

auto AsioServerFixture::socket(const std::size_t index) -> net::SocketShPtr
{
  return m_server->socket(index);
}

auto AsioServerFixture::port() const -> int
{
  return m_server->port();
}

void AsioServerFixture::writeTo(const std::size_t index, const std::string &data)
{
  auto sender = DataSender::create(data);
  sender->writeTo(*this->socket(index));
  this->waitForABit();
}

void AsioServerFixture::waitForABit()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

} // namespace test
