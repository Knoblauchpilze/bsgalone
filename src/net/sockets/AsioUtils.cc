
#include "AsioUtils.hh"
#include <sstream>

namespace net::details {

auto str(const asio::ip::tcp::endpoint &endpoint) -> std::string
{
  std::stringstream out;
  out << endpoint;
  return out.str();
}

} // namespace net::details
