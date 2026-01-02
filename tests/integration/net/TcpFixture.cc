#include "TcpFixture.hh"

namespace test {

std::atomic_int TcpFixture::NEXT_PORT{2000};

TcpFixture::TcpFixture()
  : m_port(NEXT_PORT.fetch_add(1))
{}

void TcpFixture::SetUp() {}

void TcpFixture::TearDown() {}

auto TcpFixture::port() const -> int
{
  return m_port;
}

auto TcpFixture::connectToRunningServer() -> net::SocketShPtr
{
  asio::ip::tcp::socket socket(m_context);
  auto out = std::make_shared<asio::ip::tcp::socket>(std::move(socket));

  asio::ip::tcp::resolver resolver(m_context);
  auto endpoints = resolver.resolve("127.0.0.1", std::to_string(m_port));

  asio::connect(*out, endpoints.begin(), endpoints.end());

  return out;
}

void TcpFixture::write(net::SocketShPtr socket, const std::string &data)
{
  const auto transferred = asio::write(*socket, asio::buffer(data.data(), data.size()));
  if (transferred != data.size())
  {
    throw std::runtime_error("Failed to transfer all bytes of " + data + ", only transferred "
                             + std::to_string(transferred));
  }
}

} // namespace test
