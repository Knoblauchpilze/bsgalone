
#include "AsioNetFixture.hh"
#include "DataSender.hh"
#include <chrono>

namespace test {

void AsioNetFixture::SetUp()
{
  m_server = TestTcpServer::create();
}

void AsioNetFixture::TearDown()
{
  m_server->stop();
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

auto AsioNetFixture::port() const -> int
{
  return m_server->port();
}

void AsioNetFixture::writeTo(const std::size_t index, const std::string &data)
{
  auto sender = DataSender::create(data);
  sender->writeTo(*this->socket(index));
  this->waitForABit();
}

void AsioNetFixture::waitForABit()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

} // namespace test
