
#pragma once

#include <array>
#include <string>

namespace bsgo {

enum class MessageType
{
  DOCK,
  HANGAR,
  JUMP,
  LOGIN,
  LOOT,
  PURCHASE,
  SCANNED,
  SLOT,
  VELOCITY,
  TARGET
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 10>;

} // namespace bsgo
