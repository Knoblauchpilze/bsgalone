
#include "MessageType.hh"

namespace bsgo {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::DOCK:
      return "dock";
    case MessageType::HANGAR:
      return "hangar";
    case MessageType::JUMP:
      return "jump";
    case MessageType::LOGIN:
      return "login";
    case MessageType::LOOT:
      return "loot";
    case MessageType::SCANNED:
      return "scanned";
    case MessageType::SLOT:
      return "slot";
    case MessageType::VELOCITY:
      return "velocity";
    case MessageType::TARGET:
      return "target";
    default:
      return "unknown";
  }
}

auto allMessageTypes() -> std::array<MessageType, 9>
{
  return {MessageType::DOCK,
          MessageType::HANGAR,
          MessageType::JUMP,
          MessageType::LOOT,
          MessageType::LOGIN,
          MessageType::SCANNED,
          MessageType::SLOT,
          MessageType::VELOCITY,
          MessageType::TARGET};
}

} // namespace bsgo
