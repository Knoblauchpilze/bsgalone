
#include "MessageType.hh"
#include <iostream>

namespace bsgo {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::CONNECTION:
      return "connection";
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
    case MessageType::JUMP_REQUESTED:
      return "jump_requested";
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
    case MessageType::SLOT_COMPONENT_UPDATED:
      return "slot_component_updated";
    case MessageType::VELOCITY:
      return "velocity";
    case MessageType::TARGET:
      return "target";
    case MessageType::WEAPON_COMPONENT_UPDATED:
      return "weapon_component_updated";
    default:
      return "unknown";
  }
}

auto allMessageTypes() -> std::array<MessageType, 17>
{
  return {MessageType::CONNECTION,
          MessageType::DOCK,
          MessageType::EQUIP,
          MessageType::HANGAR,
          MessageType::JUMP,
          MessageType::JUMP_CANCELLED,
          MessageType::JUMP_REQUESTED,
          MessageType::LOOT,
          MessageType::PURCHASE,
          MessageType::LOGIN,
          MessageType::SCANNED,
          MessageType::SIGNUP,
          MessageType::SLOT,
          MessageType::SLOT_COMPONENT_UPDATED,
          MessageType::VELOCITY,
          MessageType::TARGET,
          MessageType::WEAPON_COMPONENT_UPDATED};
}

auto allMessageTypesAsSet() -> std::unordered_set<MessageType>
{
  const auto all = allMessageTypes();
  return std::unordered_set<MessageType>{all.cbegin(), all.cend()};
}

} // namespace bsgo
