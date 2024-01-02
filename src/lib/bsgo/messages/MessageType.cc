
#include "MessageType.hh"

namespace bsgo {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::SCANNED:
      return "scanned";
    case MessageType::LOOT:
      return "loot";
    case MessageType::JUMP:
      return "jump";
    case MessageType::DOCK:
      return "dock";
    default:
      return "unknown";
  }
}

auto allMessageTypes() -> std::array<MessageType, 4>
{
  return {MessageType::SCANNED, MessageType::LOOT, MessageType::JUMP, MessageType::DOCK};
}

} // namespace bsgo
