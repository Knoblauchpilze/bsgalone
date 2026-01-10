
#include "MessageId.hh"

namespace net {

auto str(const MessageId id) -> std::string
{
  return std::to_string(id);
}

} // namespace net
