
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
  JUMP_CANCELLED,
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
auto allMessageTypes() -> std::array<MessageType, 13>;

} // namespace bsgo
