
#include "MessageType.hh"

namespace bsgo {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::COMPONENT_SYNC:
      return "component_sync";
    case MessageType::CONNECTION:
      return "connection";
    case MessageType::DOCK:
      return "dock";
    case MessageType::ENTITY_ADDED:
      return "entity_added";
    case MessageType::ENTITY_REMOVED:
      return "entity_removed";
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
    case MessageType::LOGOUT:
      return "logout";
    case MessageType::LOOT:
      return "loot";
    case MessageType::PLAYER_LIST:
      return "player_list";
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

auto allMessageTypes() -> std::array<MessageType, 22>
{
  return {MessageType::COMPONENT_SYNC,
          MessageType::CONNECTION,
          MessageType::DOCK,
          MessageType::ENTITY_ADDED,
          MessageType::ENTITY_REMOVED,
          MessageType::EQUIP,
          MessageType::HANGAR,
          MessageType::JUMP,
          MessageType::JUMP_CANCELLED,
          MessageType::JUMP_REQUESTED,
          MessageType::LOOT,
          MessageType::PLAYER_LIST,
          MessageType::PURCHASE,
          MessageType::LOGIN,
          MessageType::LOGOUT,
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
