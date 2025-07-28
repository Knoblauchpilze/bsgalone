
#include "MessageType.hh"

namespace bsgo {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::ASTEROID_LIST:
      return "asteroid_list";
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
    case MessageType::LOADING_FINISHED:
      return "loading_finished";
    case MessageType::LOADING_STARTED:
      return "loading_started";
    case MessageType::LOGIN:
      return "login";
    case MessageType::LOGOUT:
      return "logout";
    case MessageType::LOOT:
      return "loot";
    case MessageType::OUTPOST_LIST:
      return "outpost_list";
    case MessageType::PLAYER_LIST:
      return "player_list";
    case MessageType::PLAYER_LOGIN_DATA:
      return "player_login_data";
    case MessageType::PLAYER_SHIP_LIST:
      return "player_ship_list";
    case MessageType::PLAYER_RESOURCE_LIST:
      return "player_resource_list";
    case MessageType::PURCHASE:
      return "purchase";
    case MessageType::RESOURCE_LIST:
      return "resource_list";
    case MessageType::SCANNED:
      return "scanned";
    case MessageType::SIGNUP:
      return "signup";
    case MessageType::SLOT:
      return "slot";
    case MessageType::SLOT_COMPONENT_UPDATED:
      return "slot_component_updated";
    case MessageType::SYSTEM_LIST:
      return "system_list";
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

auto allMessageTypes() -> std::array<MessageType, 31>
{
  return {MessageType::ASTEROID_LIST,
          MessageType::COMPONENT_SYNC,
          MessageType::CONNECTION,
          MessageType::DOCK,
          MessageType::ENTITY_ADDED,
          MessageType::ENTITY_REMOVED,
          MessageType::EQUIP,
          MessageType::HANGAR,
          MessageType::JUMP,
          MessageType::JUMP_CANCELLED,
          MessageType::JUMP_REQUESTED,
          MessageType::LOADING_FINISHED,
          MessageType::LOADING_STARTED,
          MessageType::LOOT,
          MessageType::LOGIN,
          MessageType::LOGOUT,
          MessageType::OUTPOST_LIST,
          MessageType::PLAYER_LIST,
          MessageType::PLAYER_LOGIN_DATA,
          MessageType::PLAYER_RESOURCE_LIST,
          MessageType::PLAYER_SHIP_LIST,
          MessageType::PURCHASE,
          MessageType::RESOURCE_LIST,
          MessageType::SCANNED,
          MessageType::SIGNUP,
          MessageType::SLOT,
          MessageType::SLOT_COMPONENT_UPDATED,
          MessageType::SYSTEM_LIST,
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
