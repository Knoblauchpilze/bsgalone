
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
  SLOT,
  VELOCITY,
  TARGET
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 11>;

auto operator<<(std::ostream &out, const MessageType &type) -> std::ostream &;
auto operator>>(std::istream &in, MessageType &type) -> std::istream &;

} // namespace bsgo
