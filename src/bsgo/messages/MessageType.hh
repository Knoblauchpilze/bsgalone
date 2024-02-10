
#pragma once

#include <array>
#include <string>
#include <unordered_set>

namespace bsgo {

enum class MessageType
{
  CONNECTION,
  DOCK,
  ENTITY_REMOVED,
  EQUIP,
  HANGAR,
  JUMP,
  JUMP_CANCELLED,
  JUMP_REQUESTED,
  LOGIN,
  LOGOUT,
  LOOT,
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
auto allMessageTypes() -> std::array<MessageType, 19>;

auto allMessageTypesAsSet() -> std::unordered_set<MessageType>;

} // namespace bsgo
