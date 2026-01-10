
#include "ConnectedSockets.hh"

namespace test {
namespace {
auto readFrom(asio::ip::tcp::socket &socket, const std::size_t length) -> std::string
{
  std::vector<char> actual(length, 0);
  const auto received = asio::read(socket, asio::buffer(actual.data(), actual.size()));

  if (received != length)
  {
    throw std::runtime_error("Failed to receive all bytes, expected " + std::to_string(length)
                             + ", only received " + std::to_string(received));
  }

  return std::string(actual.begin(), actual.end());
}
} // namespace

auto ConnectedSockets::readServer(const std::size_t length) -> std::string
{
  return readFrom(*server, length);
}

auto ConnectedSockets::readClient(const std::size_t length) -> std::string
{
  return readFrom(*client, length);
}

void ConnectedSockets::writeServer(const std::string &data)
{
  const auto transferred = asio::write(*server, asio::buffer(data.data(), data.size()));
  if (transferred != data.size())
  {
    throw std::runtime_error("Failed to transfer all bytes of " + data + ", only transferred "
                             + std::to_string(transferred));
  }
}

} // namespace test
