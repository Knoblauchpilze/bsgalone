
#include "SignupService.hh"

namespace bsgo {

SignupService::SignupService(const Repositories &repositories)
  : AbstractService("signup", repositories)
{}

auto SignupService::trySignup(const std::string &name,
                              const std::string &password,
                              const Faction &faction) const -> std::optional<Player>
{
  const auto maybePlayer = m_repositories.playerRepository->findOneByName(name);
  if (maybePlayer)
  {
    warn("Player with name \"" + name + "\" already exists");
    return {};
  }

  Player player{.name = name, .password = password, .faction = faction};

  player.id = registerPlayer(player);
  registerResources(player);
  registerShip(player);

  return player;
}

auto SignupService::getPlayerSystemDbId(const Uuid playerDbId) const -> Uuid
{
  return m_repositories.playerRepository->findSystemByPlayer(playerDbId);
}

auto SignupService::registerPlayer(const Player &player) const -> Uuid
{
  m_repositories.playerRepository->save(player);
  const auto dbPlayer = m_repositories.playerRepository->findOneByName(player.name);

  if (!dbPlayer)
  {
    error("Failed to register player");
  }
  return dbPlayer->id;
}

namespace {
constexpr auto TYLIUM_NAME         = "tylium";
constexpr auto TYLIUM_START_AMOUNT = 10000;

constexpr auto TITANE_NAME         = "titane";
constexpr auto TITANE_START_AMOUNT = 10000;
} // namespace

void SignupService::registerResources(const Player &player) const
{
  PlayerResource data{
    .player = player.id,
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

void SignupService::registerShip(const Player &player) const
{
  const auto shipTemplate = m_repositories.shipRepository
                              ->findOneByFactionAndStarting(player.faction, true);

  PlayerShip ship{
    .ship        = shipTemplate.id,
    .name        = shipTemplate.name,
    .player      = player.id,
    .active      = true,
    .hullPoints  = shipTemplate.maxHullPoints,
    .powerPoints = shipTemplate.maxPowerPoints,
  };

  m_repositories.playerShipRepository->create(ship);

  const auto ships = m_repositories.playerShipRepository->findAllByPlayer(player.id);
  if (1u != ships.size())
  {
    error("Expected only one ship for new player " + str(player.id));
  }
  const auto dbShip = *ships.begin();

  const auto startingSystem = m_repositories.systemRepository->findOneByFactionAndStarting(
    player.faction);
  m_repositories.systemRepository->updateSystemForShip(dbShip, startingSystem, true);
}

} // namespace bsgo
