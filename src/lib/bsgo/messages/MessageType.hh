
#pragma once

#include <array>
#include <string>

namespace bsgo {

enum class MessageType
{
  SYSTEM,
  COUNT
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 1>;

} // namespace bsgo
