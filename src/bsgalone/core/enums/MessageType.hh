
#pragma once

#include <string>
#include <unordered_set>

namespace bsgalone::core {

enum class MessageType
{
  AI_BEHAVIOR_SYNC,
  ASTEROID_LIST,
  COMPONENT_SYNC,
  COMPUTER_LIST,
  CONNECTION,
  DOCK,
  ENTITY_ADDED,
  ENTITY_REMOVED,
  EQUIP,
  HANGAR,
  JOIN_SHIP,
  JUMP,
  JUMP_CANCELLED,
  JUMP_REQUESTED,
  LOADING_FINISHED,
  LOADING_STARTED,
  LOGIN,
  LOGOUT,
  LOOT,
  OUTPOST_LIST,
  PLAYER_COMPUTER_LIST,
  PLAYER_LIST,
  PLAYER_LOGIN_DATA,
  PLAYER_RESOURCE_LIST,
  PLAYER_SHIP_LIST,
  PLAYER_WEAPON_LIST,
  PURCHASE,
  RESOURCE_LIST,
  SCANNED,
  SIGNUP,
  SHIP_LIST,
  SLOT,
  SLOT_COMPONENT_UPDATED,
  SYSTEM_DATA,
  SYSTEM_LIST,
  VELOCITY,
  TARGET,
  TARGET_LIST,
  WEAPON_COMPONENT_UPDATED,
  WEAPON_LIST
};

auto str(const MessageType &type) -> std::string;

auto allMessageTypesAsSet() -> std::unordered_set<MessageType>;

} // namespace bsgalone::core
