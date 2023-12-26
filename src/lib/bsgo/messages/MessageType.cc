
#include "MessageType.hh"

namespace bsgo {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::SYSTEM:
      return "system";
    default:
      return "unknown";
  }
}

auto allMessageTypes() -> std::array<MessageType, 1>
{
  return {MessageType::SYSTEM};
}

} // namespace bsgo
