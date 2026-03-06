
#include "ShipDbView.hh"
#include "DockMessage.hh"
#include "EquipMessage.hh"
#include "HangarMessage.hh"
#include "JumpCancelledMessage.hh"
#include "JumpRequestedMessage.hh"
#include "VelocityMessage.hh"

namespace bsgalone::client {

ShipDbView::ShipDbView(GameSessionShPtr gameSession,
                       core::IMessageQueue *const internalMessageQueue,
                       core::IMessageQueue *const outputMessageQueue)
  : AbstractView("ship",
                 {
                   core::MessageType::HANGAR,
                   core::MessageType::JUMP,
                   core::MessageType::JUMP_CANCELLED,
                   core::MessageType::JUMP_REQUESTED,
                 })
  , m_gameSession(std::move(gameSession))
  , m_internalMessageQueue(internalMessageQueue)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_gameSession)
  {
    throw std::invalid_argument("Expected non null game session");
  }
  if (nullptr == m_internalMessageQueue)
  {
    throw std::invalid_argument("Expected non null internal message queue");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null output message queue");
  }
}

bool ShipDbView::isReady() const noexcept
{
  return m_gameSession->hasPlayerActiveShipDbId() && m_gameSession->hasSystemDbId()
         && m_playerShip.has_value();
}

void ShipDbView::reset()
{
  m_playerShip.reset();
}

auto ShipDbView::getPlayerShipDbId() const -> core::Uuid
{
  return m_gameSession->getPlayerActiveShipDbId();
}

void ShipDbView::dockPlayerShip() const
{
  const auto playerDbId     = m_gameSession->getPlayerDbId();
  const auto systemDbId     = m_gameSession->getSystemDbId();
  const auto playerShipDbId = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<core::DockMessage>(playerDbId,
                                                     systemDbId,
                                                     playerShipDbId,
                                                     core::DockTransition::DOCK);
  m_outputMessageQueue->pushEvent(std::move(message));
}

void ShipDbView::undockPlayerShip() const
{
  const auto playerDbId     = m_gameSession->getPlayerDbId();
  const auto systemDbId     = m_gameSession->getSystemDbId();
  const auto playerShipDbId = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<core::DockMessage>(playerDbId,
                                                     systemDbId,
                                                     playerShipDbId,
                                                     core::DockTransition::UNDOCK);
  m_outputMessageQueue->pushEvent(std::move(message));
}

void ShipDbView::returnToOutpost() const
{
  const auto playerDbId     = m_gameSession->getPlayerDbId();
  const auto systemDbId     = m_gameSession->getSystemDbId();
  const auto playerShipDbId = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<core::DockMessage>(playerDbId,
                                                     systemDbId,
                                                     playerShipDbId,
                                                     core::DockTransition::BACK_TO_OUTPOST);
  m_outputMessageQueue->pushEvent(std::move(message));
}

void ShipDbView::setJumpSystem(const core::Uuid system)
{
  m_systemToJumpTo = system;
}

void ShipDbView::clearJumpSystem()
{
  m_systemToJumpTo.reset();
}

namespace {
bool isJumping(const core::PlayerShipData &playerShip)
{
  return playerShip.jumpSystem.has_value();
}
} // namespace

void ShipDbView::startJump() const
{
  if (isJumping(*m_playerShip))
  {
    return;
  }
  if (!m_systemToJumpTo)
  {
    return;
  }

  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<core::JumpRequestedMessage>(playerDbId,
                                                              systemDbId,
                                                              shipDbId,
                                                              *m_systemToJumpTo);
  m_outputMessageQueue->pushEvent(std::move(message));
}

void ShipDbView::cancelJump() const
{
  if (!isJumping(*m_playerShip))
  {
    return;
  }

  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<core::JumpCancelledMessage>(playerDbId, systemDbId, shipDbId);
  m_outputMessageQueue->pushEvent(std::move(message));
}

void ShipDbView::accelerateShip(const Eigen::Vector3f &acceleration) const
{
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<core::VelocityMessage>(playerDbId,
                                                         systemDbId,
                                                         shipDbId,
                                                         acceleration);
  m_outputMessageQueue->pushEvent(message->clone());
  m_internalMessageQueue->pushEvent(std::move(message));
}

void ShipDbView::tryEquipItem(const core::Item &itemType, const core::Uuid itemDbId) const
{
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  m_outputMessageQueue->pushEvent(std::make_unique<core::EquipMessage>(playerDbId,
                                                                       systemDbId,
                                                                       core::EquipType::EQUIP,
                                                                       shipDbId,
                                                                       itemType,
                                                                       itemDbId));
}

void ShipDbView::tryUnequipItem(const core::Item &itemType, const core::Uuid itemDbId) const
{
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  m_outputMessageQueue->pushEvent(std::make_unique<core::EquipMessage>(playerDbId,
                                                                       systemDbId,
                                                                       core::EquipType::UNEQUIP,
                                                                       shipDbId,
                                                                       itemType,
                                                                       itemDbId));
}

auto ShipDbView::getPlayerShipWeapons() const -> std::vector<core::PlayerWeaponData>
{
  return m_playerShip->weapons;
}

auto ShipDbView::getPlayerShipComputers() const -> std::vector<core::PlayerComputerData>
{
  return m_playerShip->computers;
}

auto ShipDbView::getPlayerShipSlots() const -> std::unordered_map<core::Slot, int>
{
  return m_playerShip->slots;
}

bool ShipDbView::canStillEquipItem(const core::Item &type) const
{
  int totalSlots = 0;
  int usedSlots  = 0;

  switch (type)
  {
    case core::Item::COMPUTER:
      totalSlots = m_playerShip->slots.at(core::Slot::COMPUTER);
      usedSlots  = static_cast<int>(m_playerShip->computers.size());
      break;
    case core::Item::WEAPON:
      totalSlots = m_playerShip->slots.at(core::Slot::WEAPON);
      usedSlots  = static_cast<int>(m_playerShip->weapons.size());
      break;
    default:
      error("Unsupported item type " + str(type));
  }

  const auto canEquip = usedSlots < totalSlots;
  return canEquip;
}

void ShipDbView::handleMessageInternal(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::HANGAR:
      m_playerShip = message.as<core::HangarMessage>().getShip();
      debug("Active ship is now " + core::str(m_playerShip->dbId));
      break;
    case core::MessageType::JUMP_REQUESTED:
      m_playerShip->jumpSystem = message.as<core::JumpRequestedMessage>().getDestinationSystem();
      break;
    case core::MessageType::JUMP_CANCELLED:
    case core::MessageType::JUMP:
      m_playerShip->jumpSystem.reset();
      break;
    default:
      error("Unsupported message type " + str(message.type()));
      return;
  }
}

} // namespace bsgalone::client
