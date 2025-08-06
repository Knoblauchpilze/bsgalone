
#include "ShipDbView.hh"
#include "DockMessage.hh"
#include "EquipMessage.hh"
#include "HangarMessage.hh"
#include "JumpCancelledMessage.hh"
#include "JumpRequestedMessage.hh"
#include "LockerUtils.hh"
#include "VelocityMessage.hh"

namespace pge {

ShipDbView::ShipDbView(const bsgo::Repositories &repositories,
                       GameSessionShPtr gameSession,
                       bsgo::IMessageQueue *const internalMessageQueue,
                       bsgo::IMessageQueue *const outputMessageQueue)
  : AbstractView("ship")
  , m_repositories(repositories)
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
  return m_gameSession->hasPlayerActiveShipDbId() && m_gameSession->hasSystemDbId();
}

void ShipDbView::onMessageReceived(const bsgo::IMessage &message)
{
  const auto hangarMessage = message.as<bsgo::HangarMessage>();
  // TODO: Handle hangar message.
  warn("should handle hangar for " + bsgo::str(hangarMessage.getShipDbId()));
}

auto ShipDbView::getPlayerShipDbId() const -> bsgo::Uuid
{
  return m_gameSession->getPlayerActiveShipDbId();
}

void ShipDbView::dockPlayerShip() const
{
  const auto playerShipDbId = m_gameSession->getPlayerActiveShipDbId();
  const auto systemDbId     = m_gameSession->getSystemDbId();

  auto message = std::make_unique<bsgo::DockMessage>(playerShipDbId, true, systemDbId);
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipDbView::undockPlayerShip() const
{
  const auto playerShipDbId = m_gameSession->getPlayerActiveShipDbId();
  const auto systemDbId     = m_gameSession->getSystemDbId();

  auto message = std::make_unique<bsgo::DockMessage>(playerShipDbId, false, systemDbId);
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

bool isJumping(const bsgo::Uuid shipDbId, const bsgo::PlayerShipRepository &repository)
{
  const auto ship = repository.findOneById(shipDbId);
  return ship.jumpSystem.has_value();
}
} // namespace

void ShipDbView::startJump() const
{
  if (isJumping(m_gameSession->getPlayerActiveShipDbId(), *m_repositories.playerShipRepository))
  {
    return;
  }
  if (!m_systemToJumpTo)
  {
    return;
  }

  auto message
    = std::make_unique<bsgo::JumpRequestedMessage>(m_gameSession->getPlayerActiveShipDbId(),
                                                   *m_systemToJumpTo);
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipDbView::cancelJump() const
{
  if (!isJumping(m_gameSession->getPlayerActiveShipDbId(), *m_repositories.playerShipRepository))
  {
    return;
  }

  auto message = std::make_unique<bsgo::JumpCancelledMessage>(
    m_gameSession->getPlayerActiveShipDbId());
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipDbView::accelerateShip(const Eigen::Vector3f &acceleration) const
{
  auto message = std::make_unique<bsgo::VelocityMessage>(m_gameSession->getPlayerActiveShipDbId(),
                                                         acceleration);
  m_outputMessageQueue->pushMessage(message->clone());
  m_internalMessageQueue->pushMessage(std::move(message));
}

void ShipDbView::tryEquipItem(const bsgo::Item &itemType, const bsgo::Uuid itemDbId) const
{
  m_outputMessageQueue->pushMessage(
    std::make_unique<bsgo::EquipMessage>(bsgo::EquipType::EQUIP,
                                         m_gameSession->getPlayerActiveShipDbId(),
                                         itemType,
                                         itemDbId));
}

void ShipDbView::tryUnequipItem(const bsgo::Item &itemType, const bsgo::Uuid itemDbId) const
{
  m_outputMessageQueue->pushMessage(
    std::make_unique<bsgo::EquipMessage>(bsgo::EquipType::UNEQUIP,
                                         m_gameSession->getPlayerActiveShipDbId(),
                                         itemType,
                                         itemDbId));
}

auto ShipDbView::getPlayerShipWeapons() const -> std::vector<bsgo::PlayerWeapon>
{
  const auto weapons = m_repositories.shipWeaponRepository->findAllByShip(
    m_gameSession->getPlayerActiveShipDbId());

  std::vector<bsgo::PlayerWeapon> out;
  for (const auto &weapon : weapons)
  {
    const auto data = m_repositories.playerWeaponRepository->findOneById(weapon.weapon);
    out.push_back(data);
  }
  return out;
}

auto ShipDbView::getPlayerShipComputers() const -> std::vector<bsgo::PlayerComputer>
{
  const auto ids = m_repositories.shipComputerRepository->findAllByShip(
    m_gameSession->getPlayerActiveShipDbId());

  std::vector<bsgo::PlayerComputer> out;
  for (const auto &id : ids)
  {
    const auto computer = m_repositories.playerComputerRepository->findOneById(id);
    out.push_back(computer);
  }
  return out;
}

auto ShipDbView::getPlayerShipSlots() const -> std::unordered_map<bsgo::Slot, int>
{
  const auto ship = m_repositories.playerShipRepository->findOneById(
    m_gameSession->getPlayerActiveShipDbId());
  return ship.slots;
}

bool ShipDbView::canStillEquipItem(const bsgo::Item &type) const
{
  bool equipable{false};
  if (bsgo::Item::WEAPON == type)
  {
    bsgo::EquipData data{.shipId           = m_gameSession->getPlayerActiveShipDbId(),
                         .shipWeaponRepo   = m_repositories.shipWeaponRepository,
                         .shipComputerRepo = m_repositories.shipComputerRepository,
                         .playerShipRepo   = m_repositories.playerShipRepository};
    equipable = canStillEquipWeapon(data);
  }
  if (bsgo::Item::COMPUTER == type)
  {
    bsgo::EquipData data{.shipId           = m_gameSession->getPlayerActiveShipDbId(),
                         .shipWeaponRepo   = m_repositories.shipWeaponRepository,
                         .shipComputerRepo = m_repositories.shipComputerRepository,
                         .playerShipRepo   = m_repositories.playerShipRepository};
    equipable = canStillEquipComputer(data);
  }

  return equipable;
}

} // namespace pge
