
#include "AsioSocket.hh"
#include "AsioUtils.hh"

namespace net::details {

AsioSocket::AsioSocket(SocketShPtr socket)
  : ISocket()
  , m_socket(socket)
  , m_reader(ReadingSocket::fromSocket(socket))
  , m_writer(WritingSocket::fromSocket(socket))
{
  if (m_socket == nullptr)
  {
    throw std::invalid_argument("Expected non null asio socket");
  }

  m_reader->connect();
}

auto AsioSocket::create(asio::ip::tcp::socket socket) -> AsioSocketPtr
{
  auto shSocket = std::make_shared<asio::ip::tcp::socket>(std::move(socket));
  return std::make_unique<AsioSocket>(std::move(shSocket));
}

auto AsioSocket::endpoint() const -> std::string
{
  const auto connected = isConnected();
  const auto status    = (connected ? "ON" : "OFF");

  std::error_code code;
  const auto endpoint = m_socket->remote_endpoint(code);
  if (code)
  {
    return std::format("{}-N/A", status);
  }
  else
  {
    return std::format("{}-{}", status, net::str(endpoint));
  }
}

bool AsioSocket::isConnected() const
{
  return m_socket->is_open() && m_reader->isConnected() && m_writer->isConnected();
}

void AsioSocket::close()
{
  m_socket->close();
}

void AsioSocket::connect()
{
  m_reader->connect();
}

void AsioSocket::send(std::vector<char> bytes)
{
  m_writer->send(std::move(bytes));
}

auto AsioSocket::read() -> std::vector<char>
{
  return m_reader->read();
}

} // namespace net::details
