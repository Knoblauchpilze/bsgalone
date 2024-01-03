
#include "MessageType.hh"

namespace bsgo {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::DOCK:
      return "dock";
    case MessageType::JUMP:
      return "jump";
    case MessageType::LOOT:
      return "loot";
    case MessageType::SCANNED:
      return "scanned";
    case MessageType::SLOT:
      return "slot";
    default:
      return "unknown";
  }
}

auto allMessageTypes() -> std::array<MessageType, 5>
{
  return {MessageType::DOCK,
          MessageType::JUMP,
          MessageType::LOOT,
          MessageType::SCANNED,
          MessageType::SLOT};
}

} // namespace bsgo
