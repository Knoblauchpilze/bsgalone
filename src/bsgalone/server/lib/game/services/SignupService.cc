
#include "SignupService.hh"

namespace bsgalone::server {

SignupService::SignupService(const core::Repositories &repositories)
  : AbstractService("signup", repositories)
{}

auto SignupService::trySignup(const std::string &name,
                              const std::string &password,
                              const core::Faction &faction) const -> std::optional<core::Player>
{
  const auto maybeAccount = m_repositories.accountRepository->findOneByName(name);
  if (maybeAccount)
  {
    warn("Account with name \"" + name + "\" already exists");
    return {};
  }

  core::Account account{.username = name, .password = password};
  account.dbId = registerAccount(account);

  core::Player player{.account = account.dbId, .name = name, .faction = faction};
  player.dbId = registerPlayer(player);

  registerResources(player);
  registerShip(player);

  return player;
}

auto SignupService::getPlayerSystemDbId(const core::Uuid playerDbId) const -> core::Uuid
{
  return m_repositories.playerRepository->findSystemByPlayer(playerDbId);
}

auto SignupService::registerAccount(const core::Account &account) const -> core::Uuid
{
  m_repositories.accountRepository->save(account);
  const auto dbAccount = m_repositories.accountRepository->findOneByName(account.username);

  if (!dbAccount)
  {
    error("Failed to register account");
  }
  return dbAccount->dbId;
}

auto SignupService::registerPlayer(const core::Player &player) const -> core::Uuid
{
  m_repositories.playerRepository->save(player);
  const auto dbPlayer = m_repositories.playerRepository->findOneByAccount(*player.account);
  return dbPlayer.dbId;
}

namespace {
constexpr auto TYLIUM_NAME         = "tylium";
constexpr auto TYLIUM_START_AMOUNT = 10000;

constexpr auto TITANE_NAME         = "titane";
constexpr auto TITANE_START_AMOUNT = 10000;
} // namespace

void SignupService::registerResources(const core::Player &player) const
{
  core::PlayerResource data{
    .player = player.dbId,
  };

  const auto tylium = m_repositories.resourceRepository->findOneByName(TYLIUM_NAME);
  data.resource     = tylium->id;
  data.amount       = TYLIUM_START_AMOUNT;
  m_repositories.playerResourceRepository->save(data);

  const auto titane = m_repositories.resourceRepository->findOneByName(TITANE_NAME);
  data.resource     = titane->id;
  data.amount       = TITANE_START_AMOUNT;
  m_repositories.playerResourceRepository->save(data);
}

void SignupService::registerShip(const core::Player &player) const
{
  const auto shipTemplate = m_repositories.shipRepository
                              ->findOneByFactionAndStarting(player.faction, true);

  core::PlayerShip ship{
    .ship        = shipTemplate.id,
    .name        = shipTemplate.name,
    .player      = player.dbId,
    .active      = true,
    .hullPoints  = shipTemplate.maxHullPoints,
    .powerPoints = shipTemplate.maxPowerPoints,
  };

  m_repositories.playerShipRepository->create(ship);

  const auto ships = m_repositories.playerShipRepository->findAllByPlayer(player.dbId);
  if (1u != ships.size())
  {
    error("Expected only one ship for new player " + core::str(player.dbId));
  }
  const auto dbShip = *ships.begin();

  const auto startingSystem = m_repositories.systemRepository->findOneByFactionAndStarting(
    player.faction);
  m_repositories.systemRepository->updateSystemForShip(dbShip, startingSystem, true);
}

} // namespace bsgalone::server
