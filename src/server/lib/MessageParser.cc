
#include "MessageParser.hh"
#include <core_utils/SerializationUtils.hh>
#include <sstream>

#include "DockMessage.hh"
#include "EquipMessage.hh"
#include "HangarMessage.hh"
#include "JumpMessage.hh"
#include "LoginMessage.hh"
#include "LootMessage.hh"
#include "PurchaseMessage.hh"
#include "ScannedMessage.hh"
#include "SlotMessage.hh"
#include "TargetMessage.hh"
#include "VelocityMessage.hh"

namespace bsgo {

MessageParser::MessageParser()
  : utils::CoreObject("message")
{
  addModule("parser");
  setService("server");
}

namespace {
constexpr auto MESSAGE_SIZE = sizeof(std::underlying_type_t<MessageType>);

auto tryReadMessageType(std::istream &in) -> std::optional<MessageType>
{
  MessageType type{};
  if (!utils::deserialize(in, type))
  {
    return {};
  }

  return type;
}
} // namespace

auto MessageParser::tryParseMessage(const std::deque<char> &data) -> ParsingResult
{
  if (data.size() < MESSAGE_SIZE)
  {
    return {};
  }

  std::string input(data.begin(), data.end());
  std::istringstream in(input);

  const auto maybeMessageType = tryReadMessageType(in);
  if (!maybeMessageType)
  {
    warn("Failed to read message type from " + std::to_string(data.size()) + " byte(s)");
    return {};
  }

  ParsingResult out{};
  out.message = tryReadMessage(*maybeMessageType, in);
  if (out.message)
  {
    out.bytesProcessed = static_cast<int>(in.tellg()) - 1;
  }

  return out;
}

namespace {
auto readDockMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<DockMessage>();
  if (!message->deserialize(in))
  {
    return {};
  }

  return message;
}

auto readEquipMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<EquipMessage>();
  if (!message->deserialize(in))
  {
    return {};
  }

  return message;
}

auto readHangarMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<HangarMessage>();
  if (!message->deserialize(in))
  {
    return {};
  }

  return message;
}

auto readJumpMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<JumpMessage>();
  if (!message->deserialize(in))
  {
    return {};
  }

  return message;
}

auto readLoginMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<LoginMessage>();
  if (!message->deserialize(in))
  {
    return {};
  }

  return message;
}

auto readLootMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<LootMessage>();
  if (!message->deserialize(in))
  {
    return {};
  }

  return message;
}

auto readPurchaseMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<PurchaseMessage>();
  if (!message->deserialize(in))
  {
    return {};
  }

  return message;
}

auto readScannedMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<ScannedMessage>();
  if (!message->deserialize(in))
  {
    return {};
  }

  return message;
}

auto readSlotMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<SlotMessage>();
  if (!message->deserialize(in))
  {
    return {};
  }

  return message;
}

auto readTargetMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<TargetMessage>();
  if (!message->deserialize(in))
  {
    return {};
  }

  return message;
}

auto readVelocityMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<VelocityMessage>();
  if (!message->deserialize(in))
  {
    return {};
  }

  return message;
}
} // namespace

auto MessageParser::tryReadMessage(const MessageType &type, std::istream &in)
  -> std::optional<IMessagePtr>
{
  switch (type)
  {
    case MessageType::DOCK:
      return readDockMessage(in);
    case MessageType::EQUIP:
      return readEquipMessage(in);
    case MessageType::HANGAR:
      return readHangarMessage(in);
    case MessageType::JUMP:
      return readJumpMessage(in);
    case MessageType::LOGIN:
      return readLoginMessage(in);
    case MessageType::LOOT:
      return readLootMessage(in);
    case MessageType::PURCHASE:
      return readPurchaseMessage(in);
    case MessageType::SCANNED:
      return readScannedMessage(in);
    case MessageType::SLOT:
      return readSlotMessage(in);
    case MessageType::VELOCITY:
      return readVelocityMessage(in);
    case MessageType::TARGET:
      return readTargetMessage(in);
    default:
      error("Unsupported message type " + str(type));
      break;
  }

  // Redundant because of the error call above.
  return {};
}

} // namespace bsgo
