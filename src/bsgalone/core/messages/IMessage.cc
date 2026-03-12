
#include "IMessage.hh"

#include "AiBehaviorSyncMessage.hh"
#include "AsteroidListMessage.hh"
#include "ComponentSyncMessage.hh"
#include "ComputerListMessage.hh"
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
#include "ShipSelectedMessage.hh"
#include "SignupMessage.hh"
#include "SlotComponentMessage.hh"
#include "SlotMessage.hh"
#include "SystemDataMessage.hh"
#include "SystemListMessage.hh"
#include "TargetListMessage.hh"
#include "TargetMessage.hh"
#include "TargetPickedMessage.hh"
#include "VelocityMessage.hh"
#include "WeaponComponentMessage.hh"
#include "WeaponListMessage.hh"

namespace bsgalone::core {

namespace {
template<typename T>
auto serializeMessage(std::ostream &out, const T &message) -> std::ostream &
{
  out << message;
  return out;
}
} // namespace

// TODO: This is necessary because the OutputNetworkAdapter operates on IMessage.
// It would make sense to have a message serializer (similar to MessageParser)
// which could be unit tested.
auto operator<<(std::ostream &out, const IMessage &message) -> std::ostream &
{
  switch (message.type())
  {
    case MessageType::AI_BEHAVIOR_SYNC:
      return serializeMessage<AiBehaviorSyncMessage>(out, message.as<AiBehaviorSyncMessage>());
    case MessageType::ASTEROID_LIST:
      return serializeMessage<AsteroidListMessage>(out, message.as<AsteroidListMessage>());
    case MessageType::COMPONENT_SYNC:
      return serializeMessage<ComponentSyncMessage>(out, message.as<ComponentSyncMessage>());
    case MessageType::COMPUTER_LIST:
      return serializeMessage<ComputerListMessage>(out, message.as<ComputerListMessage>());
    case MessageType::DOCK:
      return serializeMessage<DockMessage>(out, message.as<DockMessage>());
    case MessageType::ENTITY_ADDED:
      return serializeMessage<EntityAddedMessage>(out, message.as<EntityAddedMessage>());
    case MessageType::ENTITY_REMOVED:
      return serializeMessage<EntityRemovedMessage>(out, message.as<EntityRemovedMessage>());
    case MessageType::EQUIP:
      return serializeMessage<EquipMessage>(out, message.as<EquipMessage>());
    case MessageType::HANGAR:
      return serializeMessage<HangarMessage>(out, message.as<HangarMessage>());
    case MessageType::JOIN_SHIP:
      return serializeMessage<JoinShipMessage>(out, message.as<JoinShipMessage>());
    case MessageType::JUMP:
      return serializeMessage<JumpMessage>(out, message.as<JumpMessage>());
    case MessageType::JUMP_CANCELLED:
      return serializeMessage<JumpCancelledMessage>(out, message.as<JumpCancelledMessage>());
    case MessageType::JUMP_REQUESTED:
      return serializeMessage<JumpRequestedMessage>(out, message.as<JumpRequestedMessage>());
    case MessageType::LOADING_FINISHED:
      return serializeMessage<LoadingFinishedMessage>(out, message.as<LoadingFinishedMessage>());
    case MessageType::LOADING_STARTED:
      return serializeMessage<LoadingStartedMessage>(out, message.as<LoadingStartedMessage>());
    case MessageType::LOGIN:
      return serializeMessage<LoginMessage>(out, message.as<LoginMessage>());
    case MessageType::LOGOUT:
      return serializeMessage<LogoutMessage>(out, message.as<LogoutMessage>());
    case MessageType::LOOT:
      return serializeMessage<LootMessage>(out, message.as<LootMessage>());
    case MessageType::OUTPOST_LIST:
      return serializeMessage<OutpostListMessage>(out, message.as<OutpostListMessage>());
    case MessageType::PLAYER_COMPUTER_LIST:
      return serializeMessage<PlayerComputerListMessage>(out,
                                                         message.as<PlayerComputerListMessage>());
    case MessageType::PLAYER_LIST:
      return serializeMessage<PlayerListMessage>(out, message.as<PlayerListMessage>());
    case MessageType::PLAYER_LOGIN_DATA:
      return serializeMessage<PlayerLoginDataMessage>(out, message.as<PlayerLoginDataMessage>());
    case MessageType::PLAYER_RESOURCE_LIST:
      return serializeMessage<PlayerResourceListMessage>(out,
                                                         message.as<PlayerResourceListMessage>());
    case MessageType::PLAYER_SHIP_LIST:
      return serializeMessage<PlayerShipListMessage>(out, message.as<PlayerShipListMessage>());
    case MessageType::PLAYER_WEAPON_LIST:
      return serializeMessage<PlayerWeaponListMessage>(out, message.as<PlayerWeaponListMessage>());
    case MessageType::PURCHASE:
      return serializeMessage<PurchaseMessage>(out, message.as<PurchaseMessage>());
    case MessageType::RESOURCE_LIST:
      return serializeMessage<ResourceListMessage>(out, message.as<ResourceListMessage>());
    case MessageType::SCANNED:
      return serializeMessage<ScannedMessage>(out, message.as<ScannedMessage>());
    case MessageType::SHIP_LIST:
      return serializeMessage<ShipListMessage>(out, message.as<ShipListMessage>());
    case MessageType::SHIP_SELECTED:
      return serializeMessage<ShipSelectedMessage>(out, message.as<ShipSelectedMessage>());
    case MessageType::SIGNUP:
      return serializeMessage<SignupMessage>(out, message.as<SignupMessage>());
    case MessageType::SLOT:
      return serializeMessage<SlotMessage>(out, message.as<SlotMessage>());
    case MessageType::SLOT_COMPONENT_UPDATED:
      return serializeMessage<SlotComponentMessage>(out, message.as<SlotComponentMessage>());
    case MessageType::SYSTEM_DATA:
      return serializeMessage<SystemDataMessage>(out, message.as<SystemDataMessage>());
    case MessageType::SYSTEM_LIST:
      return serializeMessage<SystemListMessage>(out, message.as<SystemListMessage>());
    case MessageType::VELOCITY:
      return serializeMessage<VelocityMessage>(out, message.as<VelocityMessage>());
    case MessageType::TARGET:
      return serializeMessage<TargetMessage>(out, message.as<TargetMessage>());
    case MessageType::TARGET_LIST:
      return serializeMessage<TargetListMessage>(out, message.as<TargetListMessage>());
    case MessageType::TARGET_PICKED:
      return serializeMessage<TargetPickedMessage>(out, message.as<TargetPickedMessage>());
    case MessageType::WEAPON_COMPONENT_UPDATED:
      return serializeMessage<WeaponComponentMessage>(out, message.as<WeaponComponentMessage>());
    case MessageType::WEAPON_LIST:
      return serializeMessage<WeaponListMessage>(out, message.as<WeaponListMessage>());
    default:
      throw std::invalid_argument("Unsupported message type " + str(message.type()));
      break;
  }
}

} // namespace bsgalone::core
