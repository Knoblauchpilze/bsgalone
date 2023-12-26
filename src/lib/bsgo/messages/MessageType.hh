
#pragma once

#include <string>

namespace bsgo {

enum class MessageType
{
  SYSTEM
};

auto str(const MessageType &type) -> std::string;

} // namespace bsgo
