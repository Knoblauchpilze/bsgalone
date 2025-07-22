
#include "ShipDbView.hh"
#include "DockMessage.hh"
#include "EquipMessage.hh"
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

auto ShipDbView::getPlayerShipDbId() const -> bsgo::Uuid
{
  checkPlayerShipDbIdExists();
  return m_gameSession->getPlayerActiveShipDbId();
}

bool ShipDbView::isReady() const noexcept
{
  return m_gameSession->hasPlayerActiveShipDbId();
}

void ShipDbView::dockPlayerShip() const
{
  checkPlayerShipDbIdExists();

  const auto ship = m_repositories.playerShipRepository->findOneById(
    m_gameSession->getPlayerActiveShipDbId());

  auto message = std::make_unique<bsgo::DockMessage>(m_gameSession->getPlayerActiveShipDbId(),
                                                     true,
                                                     *ship.system);
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipDbView::undockPlayerShip() const
{
  checkPlayerShipDbIdExists();

  const auto ship = m_repositories.playerShipRepository->findOneById(
    m_gameSession->getPlayerActiveShipDbId());

  auto message = std::make_unique<bsgo::DockMessage>(m_gameSession->getPlayerActiveShipDbId(),
                                                     false,
                                                     *ship.system);
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
  checkPlayerShipDbIdExists();

  if (isJumping(m_gameSession->getPlayerActiveShipDbId(), *m_repositories.playerShipRepository))
  {
    return;
  }
  if (!m_systemToJumpTo)
  {
    return;
  }

  checkPlayerShipDbIdExists();

  auto message
    = std::make_unique<bsgo::JumpRequestedMessage>(m_gameSession->getPlayerActiveShipDbId(),
                                                   *m_systemToJumpTo);
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipDbView::cancelJump() const
{
  checkPlayerShipDbIdExists();

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
  checkPlayerShipDbIdExists();

  auto message = std::make_unique<bsgo::VelocityMessage>(m_gameSession->getPlayerActiveShipDbId(),
                                                         acceleration);
  m_outputMessageQueue->pushMessage(message->clone());
  m_internalMessageQueue->pushMessage(std::move(message));
}

void ShipDbView::tryEquipItem(const bsgo::Item &itemType, const bsgo::Uuid itemDbId) const
{
  checkPlayerShipDbIdExists();
  m_outputMessageQueue->pushMessage(
    std::make_unique<bsgo::EquipMessage>(bsgo::EquipType::EQUIP,
                                         m_gameSession->getPlayerActiveShipDbId(),
                                         itemType,
                                         itemDbId));
}

void ShipDbView::tryUnequipItem(const bsgo::Item &itemType, const bsgo::Uuid itemDbId) const
{
  checkPlayerShipDbIdExists();
  m_outputMessageQueue->pushMessage(
    std::make_unique<bsgo::EquipMessage>(bsgo::EquipType::UNEQUIP,
                                         m_gameSession->getPlayerActiveShipDbId(),
                                         itemType,
                                         itemDbId));
}

auto ShipDbView::getPlayerShipWeapons() const -> std::vector<bsgo::PlayerWeapon>
{
  checkPlayerShipDbIdExists();

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
  checkPlayerShipDbIdExists();

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
  checkPlayerShipDbIdExists();

  const auto ship = m_repositories.playerShipRepository->findOneById(
    m_gameSession->getPlayerActiveShipDbId());
  return ship.slots;
}

bool ShipDbView::canStillEquipItem(const bsgo::Item &type) const
{
  checkPlayerShipDbIdExists();

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

void ShipDbView::checkPlayerShipDbIdExists() const
{
  if (!m_gameSession->hasPlayerActiveShipDbId())
  {
    error("Expected player ship db id to exist but it does not");
  }
}

} // namespace pge
