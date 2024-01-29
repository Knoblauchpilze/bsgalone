
#pragma once

#include <array>
#include <string>
#include <unordered_set>

namespace bsgo {

enum class MessageType
{
  CONNECTION,
  DOCK,
  EQUIP,
  HANGAR,
  JUMP,
  JUMP_CANCELLED,
  JUMP_REQUESTED,
  LOGIN,
  LOOT,
  PURCHASE,
  SCANNED,
  SIGNUP,
  SLOT,
  SLOT_COMPONENT_UPDATED,
  VELOCITY,
  TARGET
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 16>;

auto allMessageTypesAsSet() -> std::unordered_set<MessageType>;

} // namespace bsgo
