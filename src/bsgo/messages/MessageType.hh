
#pragma once

#include <array>
#include <string>
#include <unordered_set>

namespace bsgo {

enum class MessageType
{
  ASTEROID_LIST,
  COMPONENT_SYNC,
  CONNECTION,
  DOCK,
  ENTITY_ADDED,
  ENTITY_REMOVED,
  EQUIP,
  HANGAR,
  JUMP,
  JUMP_CANCELLED,
  JUMP_REQUESTED,
  LOADING_FINISHED,
  LOADING_STARTED,
  LOGIN,
  LOGOUT,
  LOOT,
  OUTPOST_LIST,
  PLAYER_LIST,
  PLAYER_LOGIN_DATA,
  PURCHASE,
  SCANNED,
  SHIP_LIST,
  SIGNUP,
  SLOT,
  SLOT_COMPONENT_UPDATED,
  SYSTEM_LIST,
  VELOCITY,
  TARGET,
  WEAPON_COMPONENT_UPDATED
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 29>;

auto allMessageTypesAsSet() -> std::unordered_set<MessageType>;

} // namespace bsgo
