
#include "MessageType.hh"

namespace bsgo {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::SCANNED:
      return "scanned";
    case MessageType::LOOT:
      return "loot";
    case MessageType::JUMP:
      return "jump";
    default:
      return "unknown";
  }
}

auto allMessageTypes() -> std::array<MessageType, 3>
{
  return {MessageType::SCANNED, MessageType::LOOT, MessageType::JUMP};
}

} // namespace bsgo
