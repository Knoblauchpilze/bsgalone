
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
constexpr auto MESSAGE_SIZE = sizeof(std::underlying_type_t<bsgalone::core::MessageType>);

auto tryReadMessageType(std::istream &in) -> std::optional<bsgalone::core::MessageType>
{
  bsgalone::core::MessageType type{};
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
auto readMessage(std::istream &in) -> std::optional<bsgalone::core::IMessagePtr>
{
  auto message = std::make_unique<MessageType>();
  if (!message->deserialize(in))
  {
    return {};
  }

  return message;
}
} // namespace

auto MessageParser::tryReadMessage(const bsgalone::core::MessageType &type, std::istream &in)
  -> std::optional<bsgalone::core::IMessagePtr>
{
  switch (type)
  {
    case bsgalone::core::MessageType::AI_BEHAVIOR_SYNC:
      return readMessage<AiBehaviorSyncMessage>(in);
    case bsgalone::core::MessageType::ASTEROID_LIST:
      return readMessage<AsteroidListMessage>(in);
    case bsgalone::core::MessageType::COMPONENT_SYNC:
      return readMessage<ComponentSyncMessage>(in);
    case bsgalone::core::MessageType::COMPUTER_LIST:
      return readMessage<ComputerListMessage>(in);
    case bsgalone::core::MessageType::CONNECTION:
      return readMessage<ConnectionMessage>(in);
    case bsgalone::core::MessageType::DOCK:
      return bsgalone::core::DockMessage::readFromStream(in);
    case bsgalone::core::MessageType::ENTITY_ADDED:
      return readMessage<EntityAddedMessage>(in);
    case bsgalone::core::MessageType::ENTITY_REMOVED:
      return readMessage<EntityRemovedMessage>(in);
    case bsgalone::core::MessageType::EQUIP:
      return readMessage<EquipMessage>(in);
    case bsgalone::core::MessageType::HANGAR:
      return readMessage<HangarMessage>(in);
    case bsgalone::core::MessageType::JOIN_SHIP:
      return readMessage<JoinShipMessage>(in);
    case bsgalone::core::MessageType::JUMP:
      return readMessage<JumpMessage>(in);
    case bsgalone::core::MessageType::JUMP_CANCELLED:
      return readMessage<JumpCancelledMessage>(in);
    case bsgalone::core::MessageType::JUMP_REQUESTED:
      return bsgalone::core::JumpRequestedMessage::readFromStream(in);
    case bsgalone::core::MessageType::LOADING_FINISHED:
      return readMessage<LoadingFinishedMessage>(in);
    case bsgalone::core::MessageType::LOADING_STARTED:
      return readMessage<LoadingStartedMessage>(in);
    case bsgalone::core::MessageType::LOGIN:
      return readMessage<LoginMessage>(in);
    case bsgalone::core::MessageType::LOGOUT:
      return readMessage<LogoutMessage>(in);
    case bsgalone::core::MessageType::LOOT:
      return readMessage<LootMessage>(in);
    case bsgalone::core::MessageType::OUTPOST_LIST:
      return readMessage<OutpostListMessage>(in);
    case bsgalone::core::MessageType::PLAYER_COMPUTER_LIST:
      return readMessage<PlayerComputerListMessage>(in);
    case bsgalone::core::MessageType::PLAYER_LIST:
      return readMessage<PlayerListMessage>(in);
    case bsgalone::core::MessageType::PLAYER_LOGIN_DATA:
      return readMessage<PlayerLoginDataMessage>(in);
    case bsgalone::core::MessageType::PLAYER_RESOURCE_LIST:
      return readMessage<PlayerResourceListMessage>(in);
    case bsgalone::core::MessageType::PLAYER_SHIP_LIST:
      return readMessage<PlayerShipListMessage>(in);
    case bsgalone::core::MessageType::PLAYER_WEAPON_LIST:
      return readMessage<PlayerWeaponListMessage>(in);
    case bsgalone::core::MessageType::PURCHASE:
      return readMessage<PurchaseMessage>(in);
    case bsgalone::core::MessageType::RESOURCE_LIST:
      return readMessage<ResourceListMessage>(in);
    case bsgalone::core::MessageType::SCANNED:
      return readMessage<ScannedMessage>(in);
    case bsgalone::core::MessageType::SHIP_LIST:
      return readMessage<ShipListMessage>(in);
    case bsgalone::core::MessageType::SIGNUP:
      return readMessage<SignupMessage>(in);
    case bsgalone::core::MessageType::SLOT:
      return readMessage<SlotMessage>(in);
    case bsgalone::core::MessageType::SLOT_COMPONENT_UPDATED:
      return readMessage<SlotComponentMessage>(in);
    case bsgalone::core::MessageType::SYSTEM_DATA:
      return readMessage<SystemDataMessage>(in);
    case bsgalone::core::MessageType::SYSTEM_LIST:
      return readMessage<SystemListMessage>(in);
    case bsgalone::core::MessageType::VELOCITY:
      return bsgalone::core::VelocityMessage::readFromStream(in);
    case bsgalone::core::MessageType::TARGET:
      return readMessage<TargetMessage>(in);
    case bsgalone::core::MessageType::TARGET_LIST:
      return readMessage<TargetListMessage>(in);
    case bsgalone::core::MessageType::WEAPON_COMPONENT_UPDATED:
      return readMessage<WeaponComponentMessage>(in);
    case bsgalone::core::MessageType::WEAPON_LIST:
      return readMessage<WeaponListMessage>(in);
    default:
      error("Unsupported message type " + str(type));
      break;
  }

  // Redundant because of the error call above.
  return {};
}

} // namespace bsgo
