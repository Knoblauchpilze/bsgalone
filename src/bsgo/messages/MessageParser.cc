
#include "MessageParser.hh"
#include "SerializationUtils.hh"
#include <sstream>

#include "AiBehaviorSyncMessage.hh"
#include "AsteroidListMessage.hh"
#include "ComponentSyncMessage.hh"
#include "ComputerListMessage.hh"
#include "ConnectionMessage.hh"
#include "DockMessage.hh"
#include "EntityAddedMessage.hh"
#include "EntityRemovedMessage.hh"
#include "EquipMessage.hh"
#include "HangarMessage.hh"
#include "JoinShipMessage.hh"
#include "JumpCancelledMessage.hh"
#include "JumpMessage.hh"
#include "JumpRequestedMessage.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include "LootMessage.hh"
#include "OutpostListMessage.hh"
#include "PlayerComputerListMessage.hh"
#include "PlayerListMessage.hh"
#include "PlayerLoginDataMessage.hh"
#include "PlayerResourceListMessage.hh"
#include "PlayerShipListMessage.hh"
#include "PlayerWeaponListMessage.hh"
#include "PurchaseMessage.hh"
#include "ResourceListMessage.hh"
#include "ScannedMessage.hh"
#include "ShipListMessage.hh"
#include "SignupMessage.hh"
#include "SlotComponentMessage.hh"
#include "SlotMessage.hh"
#include "SystemDataMessage.hh"
#include "SystemListMessage.hh"
#include "TargetListMessage.hh"
#include "TargetMessage.hh"
#include "VelocityMessage.hh"
#include "WeaponComponentMessage.hh"
#include "WeaponListMessage.hh"

namespace bsgo {

MessageParser::MessageParser()
  : core::CoreObject("message")
{
  addModule("parser");
  setService("server");
}

namespace {
constexpr auto MESSAGE_SIZE = sizeof(std::underlying_type_t<MessageType>);

auto tryReadMessageType(std::istream &in) -> std::optional<MessageType>
{
  MessageType type{};
  if (!core::deserialize(in, type))
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
    case MessageType::AI_BEHAVIOR_SYNC:
      return readMessage<AiBehaviorSyncMessage>(in);
    case MessageType::ASTEROID_LIST:
      return readMessage<AsteroidListMessage>(in);
    case MessageType::COMPONENT_SYNC:
      return readMessage<ComponentSyncMessage>(in);
    case MessageType::COMPUTER_LIST:
      return readMessage<ComputerListMessage>(in);
    case MessageType::CONNECTION:
      return readMessage<ConnectionMessage>(in);
    case MessageType::DOCK:
      return readMessage<DockMessage>(in);
    case MessageType::ENTITY_ADDED:
      return readMessage<EntityAddedMessage>(in);
    case MessageType::ENTITY_REMOVED:
      return readMessage<EntityRemovedMessage>(in);
    case MessageType::EQUIP:
      return readMessage<EquipMessage>(in);
    case MessageType::HANGAR:
      return readMessage<HangarMessage>(in);
    case MessageType::JOIN_SHIP:
      return readMessage<JoinShipMessage>(in);
    case MessageType::JUMP:
      return readMessage<JumpMessage>(in);
    case MessageType::JUMP_CANCELLED:
      return readMessage<JumpCancelledMessage>(in);
    case MessageType::JUMP_REQUESTED:
      return readMessage<JumpRequestedMessage>(in);
    case MessageType::LOADING_FINISHED:
      return readMessage<LoadingFinishedMessage>(in);
    case MessageType::LOADING_STARTED:
      return readMessage<LoadingStartedMessage>(in);
    case MessageType::LOGIN:
      return readMessage<LoginMessage>(in);
    case MessageType::LOGOUT:
      return readMessage<LogoutMessage>(in);
    case MessageType::LOOT:
      return readMessage<LootMessage>(in);
    case MessageType::OUTPOST_LIST:
      return readMessage<OutpostListMessage>(in);
    case MessageType::PLAYER_COMPUTER_LIST:
      return readMessage<PlayerComputerListMessage>(in);
    case MessageType::PLAYER_LIST:
      return readMessage<PlayerListMessage>(in);
    case MessageType::PLAYER_LOGIN_DATA:
      return readMessage<PlayerLoginDataMessage>(in);
    case MessageType::PLAYER_RESOURCE_LIST:
      return readMessage<PlayerResourceListMessage>(in);
    case MessageType::PLAYER_SHIP_LIST:
      return readMessage<PlayerShipListMessage>(in);
    case MessageType::PLAYER_WEAPON_LIST:
      return readMessage<PlayerWeaponListMessage>(in);
    case MessageType::PURCHASE:
      return readMessage<PurchaseMessage>(in);
    case MessageType::RESOURCE_LIST:
      return readMessage<ResourceListMessage>(in);
    case MessageType::SCANNED:
      return readMessage<ScannedMessage>(in);
    case MessageType::SHIP_LIST:
      return readMessage<ShipListMessage>(in);
    case MessageType::SIGNUP:
      return readMessage<SignupMessage>(in);
    case MessageType::SLOT:
      return readMessage<SlotMessage>(in);
    case MessageType::SLOT_COMPONENT_UPDATED:
      return readMessage<SlotComponentMessage>(in);
    case MessageType::SYSTEM_DATA:
      return readMessage<SystemDataMessage>(in);
    case MessageType::SYSTEM_LIST:
      return readMessage<SystemListMessage>(in);
    case MessageType::VELOCITY:
      return readMessage<VelocityMessage>(in);
    case MessageType::TARGET:
      return readMessage<TargetMessage>(in);
    case MessageType::TARGET_LIST:
      return readMessage<TargetListMessage>(in);
    case MessageType::WEAPON_COMPONENT_UPDATED:
      return readMessage<WeaponComponentMessage>(in);
    case MessageType::WEAPON_LIST:
      return readMessage<WeaponListMessage>(in);
    default:
      error("Unsupported message type " + str(type));
      break;
  }

  // Redundant because of the error call above.
  return {};
}

} // namespace bsgo
