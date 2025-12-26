
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

auto AsioSocket::endpoint() const -> std::string
{
  const auto connected = isConnected() ? "ON" : "OFF";
  return (connected ? "N/A" : str(m_socket->remote_endpoint()));
}

bool AsioSocket::isConnected() const
{
  return m_socket->is_open();
}

void AsioSocket::close()
{
  m_socket->close();
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
