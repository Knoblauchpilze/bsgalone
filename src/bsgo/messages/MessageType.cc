
#include "MessageType.hh"
#include <iostream>

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
    case MessageType::JUMP_CANCELLED:
      return "jump_cancelled";
    case MessageType::LOGIN:
      return "login";
    case MessageType::LOOT:
      return "loot";
    case MessageType::PURCHASE:
      return "purchase";
    case MessageType::SCANNED:
      return "scanned";
    case MessageType::SIGNUP:
      return "signup";
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

auto allMessageTypes() -> std::array<MessageType, 13>
{
  return {MessageType::DOCK,
          MessageType::EQUIP,
          MessageType::HANGAR,
          MessageType::JUMP,
          MessageType::JUMP_CANCELLED,
          MessageType::LOOT,
          MessageType::PURCHASE,
          MessageType::LOGIN,
          MessageType::SCANNED,
          MessageType::SIGNUP,
          MessageType::SLOT,
          MessageType::VELOCITY,
          MessageType::TARGET};
}

} // namespace bsgo
