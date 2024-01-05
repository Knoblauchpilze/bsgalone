
#include "MessageType.hh"

namespace bsgo {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::DOCK:
      return "dock";
    case MessageType::EQUIP:
      return "equip";
    case MessageType::HANGAR:
      return "hangar";
    case MessageType::JUMP:
      return "jump";
    case MessageType::LOGIN:
      return "login";
    case MessageType::LOOT:
      return "loot";
    case MessageType::PURCHASE:
      return "purchase";
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

auto allMessageTypes() -> std::array<MessageType, 11>
{
  return {MessageType::DOCK,
          MessageType::EQUIP,
          MessageType::HANGAR,
          MessageType::JUMP,
          MessageType::LOOT,
          MessageType::PURCHASE,
          MessageType::LOGIN,
          MessageType::SCANNED,
          MessageType::SLOT,
          MessageType::VELOCITY,
          MessageType::TARGET};
}

} // namespace bsgo
