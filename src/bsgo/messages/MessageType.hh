
#pragma once

#include <array>
#include <string>
#include <unordered_set>

namespace bsgo {

// Dummy
enum class MessageType
{
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
  LOADING_STARTED,
  LOADING_FINISHED,
  LOGIN,
  LOGOUT,
  LOOT,
  PLAYER_LIST,
  PURCHASE,
  SCANNED,
  SIGNUP,
  SLOT,
  SLOT_COMPONENT_UPDATED,
  VELOCITY,
  TARGET,
  WEAPON_COMPONENT_UPDATED
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 24>;

auto allMessageTypesAsSet() -> std::unordered_set<MessageType>;

} // namespace bsgo
