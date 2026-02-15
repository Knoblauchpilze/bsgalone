
#include "MessageType.hh"

namespace bsgalone::core {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::AI_BEHAVIOR_SYNC:
      return "ai_behavior_sync";
    case MessageType::ASTEROID_LIST:
      return "asteroid_list";
    case MessageType::COMPONENT_SYNC:
      return "component_sync";
    case MessageType::COMPUTER_LIST:
      return "computer_list";
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
    case MessageType::JOIN_SHIP:
      return "join_ship";
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
    case MessageType::PLAYER_COMPUTER_LIST:
      return "player_computer_list";
    case MessageType::PLAYER_LIST:
      return "player_list";
    case MessageType::PLAYER_LOGIN_DATA:
      return "player_login_data";
    case MessageType::PLAYER_RESOURCE_LIST:
      return "player_resource_list";
    case MessageType::PLAYER_SHIP_LIST:
      return "player_ship_list";
    case MessageType::PLAYER_WEAPON_LIST:
      return "player_weapon_list";
    case MessageType::PURCHASE:
      return "purchase";
    case MessageType::RESOURCE_LIST:
      return "resource_list";
    case MessageType::SCANNED:
      return "scanned";
    case MessageType::SHIP_LIST:
      return "ship_list";
    case MessageType::SIGNUP:
      return "signup";
    case MessageType::SLOT:
      return "slot";
    case MessageType::SLOT_COMPONENT_UPDATED:
      return "slot_component_updated";
    case MessageType::SYSTEM_DATA:
      return "system_data";
    case MessageType::SYSTEM_LIST:
      return "system_list";
    case MessageType::VELOCITY:
      return "velocity";
    case MessageType::TARGET:
      return "target";
    case MessageType::TARGET_LIST:
      return "target_list";
    case MessageType::WEAPON_COMPONENT_UPDATED:
      return "weapon_component_updated";
    case MessageType::WEAPON_LIST:
      return "weapon_list";
    default:
      return "unknown";
  }
}

auto allMessageTypesAsSet() -> std::unordered_set<MessageType>
{
  return std::unordered_set<MessageType>{
    MessageType::AI_BEHAVIOR_SYNC,
    MessageType::ASTEROID_LIST,
    MessageType::COMPONENT_SYNC,
    MessageType::COMPUTER_LIST,
    MessageType::CONNECTION,
    MessageType::DOCK,
    MessageType::ENTITY_ADDED,
    MessageType::ENTITY_REMOVED,
    MessageType::EQUIP,
    MessageType::HANGAR,
    MessageType::JOIN_SHIP,
    MessageType::JUMP,
    MessageType::JUMP_CANCELLED,
    MessageType::JUMP_REQUESTED,
    MessageType::LOADING_FINISHED,
    MessageType::LOADING_STARTED,
    MessageType::LOOT,
    MessageType::LOGIN,
    MessageType::LOGOUT,
    MessageType::OUTPOST_LIST,
    MessageType::PLAYER_COMPUTER_LIST,
    MessageType::PLAYER_LIST,
    MessageType::PLAYER_LOGIN_DATA,
    MessageType::PLAYER_RESOURCE_LIST,
    MessageType::PLAYER_SHIP_LIST,
    MessageType::PLAYER_WEAPON_LIST,
    MessageType::PURCHASE,
    MessageType::RESOURCE_LIST,
    MessageType::SCANNED,
    MessageType::SHIP_LIST,
    MessageType::SIGNUP,
    MessageType::SLOT,
    MessageType::SLOT_COMPONENT_UPDATED,
    MessageType::SYSTEM_DATA,
    MessageType::SYSTEM_LIST,
    MessageType::VELOCITY,
    MessageType::TARGET,
    MessageType::TARGET_LIST,
    MessageType::WEAPON_COMPONENT_UPDATED,
    MessageType::WEAPON_LIST,
  };
}

} // namespace bsgalone::core
