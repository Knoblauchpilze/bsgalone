
#include "LoadingMessagesConsumer.hh"
#include "AsteroidListMessage.hh"
#include "OutpostListMessage.hh"
#include "PlayerListMessage.hh"
#include "PlayerLoginDataMessage.hh"
#include "ShipListMessage.hh"
#include "SystemProcessorUtils.hh"

namespace bsgo {

LoadingMessagesConsumer::LoadingMessagesConsumer(const Services &services,
                                                 IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("loading", {MessageType::LOADING_FINISHED, MessageType::LOADING_STARTED})
  , m_loadingService(services.loading)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_loadingService)
  {
    throw std::invalid_argument("Expected non null loading service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LoadingMessagesConsumer::onMessageReceived(const IMessage &message)
{
  switch (message.type())
  {
    case MessageType::LOADING_STARTED:
      handleLoadingStartedMessage(message.as<LoadingStartedMessage>());
      return;
    case MessageType::LOADING_FINISHED:
      forwardLoadingFinishedMessage(message.as<LoadingFinishedMessage>());
      return;
    default:
      error("Unsupported loading operation " + str(message.type()));
      break;
  }
}

void LoadingMessagesConsumer::handleLoadingStartedMessage(const LoadingStartedMessage &message) const
{
  m_outputMessageQueue->pushMessage(message.clone());

  info("Transition is " + str(message.getTransition()) + " for system "
       + str(message.getSystemDbId()));

  switch (message.getTransition())
  {
    case LoadingTransition::LOGIN:
      handleLoginDataLoading(message);
      break;
    case LoadingTransition::UNDOCK:
    case LoadingTransition::JUMP:
      handlePlayersLoading(message);
      handleAsteroidsLoading(message);
      handleOutpostsLoading(message);
      handleShipsLoading(message);
      break;
    default:
      error("Unsupported loading transition " + str(message.getTransition()));
      return;
  }
}

void LoadingMessagesConsumer::forwardLoadingFinishedMessage(
  const LoadingFinishedMessage &message) const
{
  m_outputMessageQueue->pushMessage(message.clone());
}

void LoadingMessagesConsumer::handleLoginDataLoading(const LoadingStartedMessage &message) const
{
  const auto maybePlayerDbId = message.tryGetPlayerDbId();
  if (!maybePlayerDbId)
  {
    warn("Failed to process loading started message", "No player defined");
    return;
  }

  const auto props = m_loadingService->getDataForPlayer(*maybePlayerDbId);

  auto out = std::make_unique<PlayerLoginDataMessage>(props.faction, props.shipDbId, props.docked);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handlePlayersLoading(const LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto players = m_loadingService->getPlayersInSystem(systemDbId);

  std::vector<PlayerListMessage::PlayerData> playersData{};
  std::transform(players.begin(),
                 players.end(),
                 std::back_inserter(playersData),
                 [](const Player &player) {
                   return PlayerListMessage::PlayerData{.playerDbId = player.id,
                                                        .name       = player.name};
                 });

  auto out = std::make_unique<PlayerListMessage>(systemDbId, playersData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handleAsteroidsLoading(const LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto asteroids = m_loadingService->getAsteroidsInSystem(systemDbId);

  std::vector<AsteroidData> asteroidsData{};
  std::transform(asteroids.begin(),
                 asteroids.end(),
                 std::back_inserter(asteroidsData),
                 [](const LoadingService::AsteroidProps &props) {
                   return AsteroidData{
                     .dbId     = props.dbId,
                     .position = props.dbAsteroid.position,
                     .radius   = props.dbAsteroid.radius,
                     .health   = props.dbAsteroid.health,
                     .resource = props.resource,
                     .amount   = props.amount,
                   };
                 });

  auto out = std::make_unique<AsteroidListMessage>(systemDbId, asteroidsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handleOutpostsLoading(const LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto outposts = m_loadingService->getOutpostsInSystem(systemDbId);

  std::vector<OutpostData> outpostsData{};
  std::transform(outposts.begin(),
                 outposts.end(),
                 std::back_inserter(outpostsData),
                 [](const LoadingService::OutpostProps &props) {
                   return OutpostData{
                     .dbId            = props.dbId,
                     .position        = props.dbOutpost.position,
                     .radius          = props.dbOutpost.radius,
                     .hullPoints      = props.dbOutpost.hullPoints,
                     .maxHullPoints   = props.dbOutpost.maxHullPoints,
                     .hullPointsRegen = props.dbOutpost.hullPointsRegen,
                     .powerPoints     = props.dbOutpost.powerPoints,
                     .maxPowerPoints  = props.dbOutpost.maxPowerPoints,
                     .powerRegen      = props.dbOutpost.powerRegen,
                     .faction         = props.dbOutpost.faction,
                     .targetDbId      = props.targetDbId,
                   };
                 });

  auto out = std::make_unique<OutpostListMessage>(systemDbId, outpostsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

namespace {
auto generateShipData(const LoadingService::ShipProps props) -> ShipData
{
  ShipData data;

  data.dbId            = props.dbShip.id;
  data.position        = props.dbShip.position;
  data.radius          = props.dbShip.radius;
  data.acceleration    = props.dbShip.acceleration;
  data.speed           = props.dbShip.speed;
  data.hullPoints      = props.dbShip.hullPoints;
  data.maxHullPoints   = props.dbShip.maxHullPoints;
  data.hullPointsRegen = props.dbShip.hullPointsRegen;
  data.powerPoints     = props.dbShip.powerPoints;
  data.maxPowerPoints  = props.dbShip.maxPowerPoints;
  data.powerRegen      = props.dbShip.powerRegen;
  data.faction         = props.dbShip.faction;

  data.status    = props.status;
  data.shipClass = props.dbShip.shipClass;
  data.name      = props.dbShip.name;

  data.targetDbId = props.targetDbId;
  data.playerDbId = props.dbShip.player;

  for (const auto &weapon : props.weapons)
  {
    WeaponData weaponData{
      .dbId         = weapon.dbWeapon.id,
      .weaponDbId   = weapon.dbWeapon.weapon,
      .slotPosition = weapon.slotPosition,
      .level        = weapon.dbWeapon.level,
      .minDamage    = weapon.dbWeapon.minDamage,
      .maxDamage    = weapon.dbWeapon.maxDamage,
      .powerCost    = weapon.dbWeapon.powerCost,
      .range        = weapon.dbWeapon.range,
      .reloadTime   = weapon.dbWeapon.reloadTime,
    };

    data.weapons.emplace_back(std::move(weaponData));
  }

  for (const auto &computer : props.computers)
  {
    ComputerData computerData{
      .dbId           = computer.id,
      .computerDbId   = computer.computer,
      .level          = computer.level,
      .offensive      = computer.offensive,
      .powerCost      = computer.powerCost,
      .range          = computer.range,
      .reloadTime     = computer.reloadTime,
      .duration       = computer.duration,
      .allowedTargets = computer.allowedTargets,
      .damageModifier = computer.damageModifier,
    };

    data.computers.emplace_back(std::move(computerData));
  }

  return data;
}
} // namespace

void LoadingMessagesConsumer::handleShipsLoading(const LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto ships = m_loadingService->getShipsInSystem(systemDbId);

  std::vector<ShipData> shipsData{};
  std::transform(ships.begin(), ships.end(), std::back_inserter(shipsData), generateShipData);

  auto out = std::make_unique<ShipListMessage>(systemDbId, shipsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
