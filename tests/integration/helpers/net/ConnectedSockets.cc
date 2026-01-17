
#include "ConnectedSockets.hh"

namespace test {

auto ConnectedSockets::readServer(const std::size_t length) -> std::string
{
  return readFrom(*server, length);
}

auto ConnectedSockets::readClient(const std::size_t length) -> std::string
{
  return readFrom(*client, length);
}

void ConnectedSockets::drainClient(const std::size_t length)
{
  readFrom(*client, length);
}

} // namespace test
