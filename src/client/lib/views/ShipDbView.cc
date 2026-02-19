
#include "ShipDbView.hh"
#include "DockMessage.hh"
#include "EquipMessage.hh"
#include "HangarMessage.hh"
#include "JumpCancelledMessage.hh"
#include "JumpRequestedMessage.hh"
#include "VelocityMessage.hh"

namespace pge {

ShipDbView::ShipDbView(GameSessionShPtr gameSession,
                       bsgalone::core::IMessageQueue *const internalMessageQueue,
                       bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractView("ship",
                 {
                   bsgalone::core::MessageType::HANGAR,
                   bsgalone::core::MessageType::JUMP,
                   bsgalone::core::MessageType::JUMP_CANCELLED,
                   bsgalone::core::MessageType::JUMP_REQUESTED,
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

auto ShipDbView::getPlayerShipDbId() const -> bsgo::Uuid
{
  return m_gameSession->getPlayerActiveShipDbId();
}

void ShipDbView::dockPlayerShip() const
{
  const auto playerDbId     = m_gameSession->getPlayerDbId();
  const auto systemDbId     = m_gameSession->getSystemDbId();
  const auto playerShipDbId = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<bsgalone::core::DockMessage>(playerDbId,
                                                               systemDbId,
                                                               playerShipDbId,
                                                               bsgalone::core::DockTransition::DOCK);
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipDbView::undockPlayerShip() const
{
  const auto playerDbId     = m_gameSession->getPlayerDbId();
  const auto systemDbId     = m_gameSession->getSystemDbId();
  const auto playerShipDbId = m_gameSession->getPlayerActiveShipDbId();

  auto message
    = std::make_unique<bsgalone::core::DockMessage>(playerDbId,
                                                    systemDbId,
                                                    playerShipDbId,
                                                    bsgalone::core::DockTransition::UNDOCK);
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipDbView::returnToOutpost() const
{
  const auto playerDbId     = m_gameSession->getPlayerDbId();
  const auto systemDbId     = m_gameSession->getSystemDbId();
  const auto playerShipDbId = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<bsgalone::core::DockMessage>(
    playerDbId, systemDbId, playerShipDbId, bsgalone::core::DockTransition::BACK_TO_OUTPOST);
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipDbView::setJumpSystem(const bsgo::Uuid system)
{
  m_systemToJumpTo = system;
}

void ShipDbView::clearJumpSystem()
{
  m_systemToJumpTo.reset();
}

namespace {
bool isJumping(const bsgo::PlayerShipData &playerShip)
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

  auto message = std::make_unique<bsgalone::core::JumpRequestedMessage>(playerDbId,
                                                                        systemDbId,
                                                                        shipDbId,
                                                                        *m_systemToJumpTo);
  m_outputMessageQueue->pushMessage(std::move(message));
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

  auto message = std::make_unique<bsgalone::core::JumpCancelledMessage>(playerDbId,
                                                                        systemDbId,
                                                                        shipDbId);
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipDbView::accelerateShip(const Eigen::Vector3f &acceleration) const
{
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<bsgalone::core::VelocityMessage>(playerDbId,
                                                                   systemDbId,
                                                                   shipDbId,
                                                                   acceleration);
  m_outputMessageQueue->pushMessage(message->clone());
  m_internalMessageQueue->pushMessage(std::move(message));
}

void ShipDbView::tryEquipItem(const bsgalone::core::Item &itemType, const bsgo::Uuid itemDbId) const
{
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  m_outputMessageQueue->pushMessage(std::make_unique<bsgalone::core::EquipMessage>(
    playerDbId, systemDbId, bsgalone::core::EquipType::EQUIP, shipDbId, itemType, itemDbId));
}

void ShipDbView::tryUnequipItem(const bsgalone::core::Item &itemType,
                                const bsgo::Uuid itemDbId) const
{
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  m_outputMessageQueue->pushMessage(std::make_unique<bsgalone::core::EquipMessage>(
    playerDbId, systemDbId, bsgalone::core::EquipType::UNEQUIP, shipDbId, itemType, itemDbId));
}

auto ShipDbView::getPlayerShipWeapons() const -> std::vector<bsgo::PlayerWeaponData>
{
  return m_playerShip->weapons;
}

auto ShipDbView::getPlayerShipComputers() const -> std::vector<bsgo::PlayerComputerData>
{
  return m_playerShip->computers;
}

auto ShipDbView::getPlayerShipSlots() const -> std::unordered_map<bsgo::Slot, int>
{
  return m_playerShip->slots;
}

bool ShipDbView::canStillEquipItem(const bsgalone::core::Item &type) const
{
  int totalSlots = 0;
  int usedSlots  = 0;

  switch (type)
  {
    case bsgalone::core::Item::COMPUTER:
      totalSlots = m_playerShip->slots.at(bsgo::Slot::COMPUTER);
      usedSlots  = static_cast<int>(m_playerShip->computers.size());
      break;
    case bsgalone::core::Item::WEAPON:
      totalSlots = m_playerShip->slots.at(bsgo::Slot::WEAPON);
      usedSlots  = static_cast<int>(m_playerShip->weapons.size());
      break;
    default:
      error("Unsupported item type " + str(type));
  }

  const auto canEquip = usedSlots < totalSlots;
  return canEquip;
}

void ShipDbView::handleMessageInternal(const bsgalone::core::IMessage &message)
{
  switch (message.type())
  {
    case bsgalone::core::MessageType::HANGAR:
      m_playerShip = message.as<bsgo::HangarMessage>().getShip();
      debug("Active ship is now " + bsgo::str(m_playerShip->dbId));
      break;
    case bsgalone::core::MessageType::JUMP_REQUESTED:
      m_playerShip->jumpSystem = message.as<bsgalone::core::JumpRequestedMessage>()
                                   .getDestinationSystem();
      break;
    case bsgalone::core::MessageType::JUMP_CANCELLED:
    case bsgalone::core::MessageType::JUMP:
      m_playerShip->jumpSystem.reset();
      break;
    default:
      error("Unsupported message type " + str(message.type()));
      return;
  }
}

} // namespace pge
