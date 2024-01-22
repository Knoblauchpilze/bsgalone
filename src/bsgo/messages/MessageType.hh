
#pragma once

#include <array>
#include <string>

namespace bsgo {

enum class MessageType
{
  DOCK,
  EQUIP,
  HANGAR,
  JUMP,
  LOGIN,
  LOOT,
  PURCHASE,
  SCANNED,
  SIGNUP,
  SLOT,
  VELOCITY,
  TARGET
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 12>;

} // namespace bsgo
