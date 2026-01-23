
#include "TcpFixture.hh"

namespace test {

TcpFixture::TcpFixture()
  : PortFixture()
{}

void TcpFixture::SetUp()
{
  this->PortFixture::SetUp();
}

void TcpFixture::TearDown()
{
  this->PortFixture::TearDown();
}

auto TcpFixture::asioContext() -> net::details::AsioContext &
{
  return *m_context;
}

auto TcpFixture::connectToRunningServer() -> net::details::SocketShPtr
{
  asio::ip::tcp::socket socket(m_context->get());
  auto out = std::make_shared<asio::ip::tcp::socket>(std::move(socket));

  asio::ip::tcp::resolver resolver(m_context->get());
  auto endpoints = resolver.resolve("127.0.0.1", std::to_string(this->port()));

  asio::connect(*out, endpoints.begin(), endpoints.end());

  return out;
}

void TcpFixture::write(net::details::SocketShPtr socket, const std::string &data)
{
  const auto transferred = asio::write(*socket, asio::buffer(data.data(), data.size()));
  if (transferred != data.size())
  {
    throw std::runtime_error("Failed to transfer all bytes of " + data + ", only transferred "
                             + std::to_string(transferred));
  }
}

} // namespace test
