
#include "AsioNetFixture.hh"
#include "DataSender.hh"
#include <chrono>

namespace test {

void AsioNetFixture::SetUp()
{
  std::cout << "[fixture] creating tcp server\n";
  m_server = TestTcpServer::create();
  std::cout << "[fixture] fixture initialized\n";
}

void AsioNetFixture::TearDown()
{
  std::cout << "[fixture] tearing down\n";
  m_server->stop();
  m_server.reset();
  std::cout << "[fixture] fixture destroyed\n";
}

auto AsioNetFixture::connect() -> net::SocketShPtr
{
  std::cout << "[fixture] connecting to server\n";
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
