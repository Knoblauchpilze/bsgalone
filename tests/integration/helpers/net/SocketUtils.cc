
#include "SocketUtils.hh"

namespace test {

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

void writeTo(asio::ip::tcp::socket &socket, const std::vector<char> &data)
{
  const auto transferred = asio::write(socket, asio::buffer(data.data(), data.size()));
  if (transferred != data.size())
  {
    throw std::runtime_error("Failed to transfer all bytes of input data, only transferred "
                             + std::to_string(transferred) + " out of "
                             + std::to_string(data.size()));
  }
}

} // namespace test
