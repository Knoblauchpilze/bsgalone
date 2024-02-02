
#include "MessageParser.hh"
#include <core_utils/SerializationUtils.hh>
#include <sstream>

#include "ConnectionMessage.hh"
#include "DockMessage.hh"
#include "EquipMessage.hh"
#include "HangarMessage.hh"
#include "JumpCancelledMessage.hh"
#include "JumpMessage.hh"
#include "JumpRequestedMessage.hh"
#include "LoginMessage.hh"
#include "LootMessage.hh"
#include "PurchaseMessage.hh"
#include "ScannedMessage.hh"
#include "SignupMessage.hh"
#include "SlotComponentMessage.hh"
#include "SlotMessage.hh"
#include "TargetMessage.hh"
#include "VelocityMessage.hh"
#include "WeaponComponentMessage.hh"

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

  in.seekg(0, std::ios::beg);

  ParsingResult out{};
  out.message = tryReadMessage(*maybeMessageType, in);
  if (out.message)
  {
    out.bytesProcessed = static_cast<int>(in.tellg());
  }

  return out;
}

namespace {
template<typename MessageType>
auto readMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<MessageType>();
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
    case MessageType::CONNECTION:
      return readMessage<ConnectionMessage>(in);
    case MessageType::DOCK:
      return readMessage<DockMessage>(in);
    case MessageType::EQUIP:
      return readMessage<EquipMessage>(in);
    case MessageType::HANGAR:
      return readMessage<HangarMessage>(in);
    case MessageType::JUMP:
      return readMessage<JumpMessage>(in);
    case MessageType::JUMP_CANCELLED:
      return readMessage<JumpCancelledMessage>(in);
    case MessageType::JUMP_REQUESTED:
      return readMessage<JumpRequestedMessage>(in);
    case MessageType::LOGIN:
      return readMessage<LoginMessage>(in);
    case MessageType::LOOT:
      return readMessage<LootMessage>(in);
    case MessageType::PURCHASE:
      return readMessage<PurchaseMessage>(in);
    case MessageType::SCANNED:
      return readMessage<ScannedMessage>(in);
    case MessageType::SIGNUP:
      return readMessage<SignupMessage>(in);
    case MessageType::SLOT:
      return readMessage<SlotMessage>(in);
    case MessageType::SLOT_COMPONENT_UPDATED:
      return readMessage<SlotComponentMessage>(in);
    case MessageType::VELOCITY:
      return readMessage<VelocityMessage>(in);
    case MessageType::TARGET:
      return readMessage<TargetMessage>(in);
    case MessageType::WEAPON_COMPONENT_UPDATED:
      return readMessage<WeaponComponentMessage>(in);
    default:
      error("Unsupported message type " + str(type));
      break;
  }

  // Redundant because of the error call above.
  return {};
}

} // namespace bsgo
