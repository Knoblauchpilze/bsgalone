
#include "ClientId.hh"

namespace net {

using ClientId = int;

auto str(const ClientId id) -> std::string
{
  return std::to_string(id);
}

} // namespace net
