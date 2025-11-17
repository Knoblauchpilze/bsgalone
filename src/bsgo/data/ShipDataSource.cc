
#include "ShipDataSource.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"
#include "VectorUtils.hh"

#include "FallbackNode.hh"
#include "FireNode.hh"
#include "FollowTargetNode.hh"
#include "PickTargetNode.hh"
#include "RepeaterNode.hh"
#include "SequenceNode.hh"
#include "TargetNode.hh"

namespace bsgo {

ShipDataSource::ShipDataSource()
  : core::CoreObject("bsgo")
{
  setService("data");
  addModule("ship");
}

ShipDataSource::ShipDataSource(const Repositories &repositories)
  : core::CoreObject("bsgo")
  , m_repositories(repositories)
{
  setService("data");
  addModule("ship");
}

void ShipDataSource::initialize(const Uuid systemDbId,
                                Coordinator &coordinator,
                                DatabaseEntityMapper &entityMapper) const
{
  if (!m_repositories)
  {
    error("Failed to initialize ship", "Repositories are not set");
  }

  const auto ships = m_repositories->systemRepository->findAllShipsBySystem(systemDbId);
  for (const auto &id : ships)
  {
    registerShip(coordinator, id, entityMapper, true);
  }
}

void ShipDataSource::registerShip(Coordinator &coordinator,
                                  const PlayerShipData &data,
                                  DatabaseEntityMapper &entityMapper,
                                  const bool ignoreIfDocked) const
{
  if (ignoreIfDocked && data.docked)
  {
    return;
  }

  const auto shipEntityId = coordinator.createEntity(EntityKind::SHIP);

  coordinator.addDbId(shipEntityId, data.dbId);
  auto box = std::make_unique<CircleBox>(data.position, data.radius);
  coordinator.addTransform(shipEntityId, std::move(box));
  coordinator.addRemoval(shipEntityId);
  const VelocityData vData{.maxAcceleration = data.acceleration, .maxSpeed = data.speed};
  coordinator.addVelocity(shipEntityId, vData);
  coordinator.addHealth(shipEntityId, data.hullPoints, data.maxHullPoints, data.hullPointsRegen);
  coordinator.addPower(shipEntityId, data.powerPoints, data.maxPowerPoints, data.powerRegen);
  coordinator.addFaction(shipEntityId, data.faction);
  coordinator.addStatus(shipEntityId, data.status, data.jumpTime, data.jumpTimeInThreat);
  coordinator.addShipClass(shipEntityId, data.shipClass);
  coordinator.addName(shipEntityId, data.name);
  if (data.targetDbId)
  {
    // TODO: This is wrong as the coordinator expects the entity identifier
    coordinator.addTarget(shipEntityId, *data.targetDbId);
  }
  else
  {
    coordinator.addTarget(shipEntityId);
  }
  coordinator.addNetworkSync(shipEntityId,
                             {ComponentType::HEALTH,
                              ComponentType::POWER,
                              ComponentType::STATUS,
                              ComponentType::TRANSFORM,
                              ComponentType::VELOCITY});
  coordinator.addDbSync(shipEntityId);

  registerShipOwner(coordinator, shipEntityId, data, entityMapper);
  registerShipWeapons(coordinator, data, shipEntityId);
  registerShipComputers(coordinator, data, shipEntityId);
}

void ShipDataSource::registerShip(Coordinator &coordinator,
                                  const Uuid ship,
                                  DatabaseEntityMapper &entityMapper) const
{
  if (!m_repositories)
  {
    error("Failed to initialize ship", "Repositories are not set");
  }

  registerShip(coordinator, ship, entityMapper, false);
}

void ShipDataSource::registerShip(Coordinator &coordinator,
                                  const Uuid shipDbId,
                                  DatabaseEntityMapper &entityMapper,
                                  const bool ignoreDocked) const
{
  const auto data   = m_repositories->playerShipRepository->findOneById(shipDbId);
  const auto player = m_repositories->playerRepository->findOneById(data.player);

  PlayerShipData out{
    .dbId             = shipDbId,
    .shipId           = data.ship,
    .position         = data.position,
    .radius           = data.radius,
    .acceleration     = data.acceleration,
    .speed            = data.speed,
    .hullPoints       = data.hullPoints,
    .maxHullPoints    = data.maxHullPoints,
    .hullPointsRegen  = data.hullPointsRegen,
    .powerPoints      = data.powerPoints,
    .maxPowerPoints   = data.maxPowerPoints,
    .powerRegen       = data.powerRegen,
    .faction          = data.faction,
    .status           = determineStartingStatusForShip(data, player),
    .shipClass        = data.shipClass,
    .name             = data.name,
    .docked           = data.docked,
    .jumpTime         = data.jumpTime,
    .jumpTimeInThreat = data.jumpTimeInThreat,

    .targetDbId = {},
    .playerDbId = data.player,

    .slots         = data.slots,
    .aiTargets     = data.aiTargets,
    .reachedTarget = data.reachedTarget,
  };

  const auto weapons = m_repositories->shipWeaponRepository->findAllByShip(shipDbId);
  for (const auto &weapon : weapons)
  {
    const auto weaponDbData = m_repositories->playerWeaponRepository->findOneById(weapon.weapon);

    PlayerWeaponData weaponData{
      .dbId         = weaponDbData.id,
      .weaponDbId   = weaponDbData.weapon,
      .slotPosition = weapon.slotPosition,
      .level        = weaponDbData.level,
      .minDamage    = weaponDbData.minDamage,
      .maxDamage    = weaponDbData.maxDamage,
      .powerCost    = weaponDbData.powerCost,
      .range        = weaponDbData.range,
      .reloadTime   = weaponDbData.reloadTime,
    };

    out.weapons.push_back(weaponData);
  }

  const auto computers = m_repositories->shipComputerRepository->findAllByShip(shipDbId);
  for (const auto &computer : computers)
  {
    const auto computerDbData = m_repositories->playerComputerRepository->findOneById(computer);

    PlayerComputerData computerData{
      .dbId         = computerDbData.id,
      .computerDbId = computerDbData.computer,

      .level      = computerDbData.level,
      .offensive  = computerDbData.offensive,
      .powerCost  = computerDbData.powerCost,
      .range      = computerDbData.range,
      .reloadTime = computerDbData.reloadTime,

      .duration       = computerDbData.duration,
      .allowedTargets = computerDbData.allowedTargets,
      .damageModifier = computerDbData.damageModifier,
    };

    out.computers.push_back(computerData);
  }

  registerShip(coordinator, out, entityMapper, ignoreDocked);
}

void ShipDataSource::registerShipOwner(Coordinator &coordinator,
                                       const Uuid shipEntity,
                                       const PlayerShipData &data,
                                       DatabaseEntityMapper &entityMapper) const
{
  // This reachedTarget effectively allows to distinguish between AI ships
  // and player ships. It is not used in this way though but only to attach
  // an AI component. The AI component could be renamed to script or similar.
  // Another possibility would be to add a isAi in the PlayerShipData but
  // so far there does not seem to be a use case for it outside of making it
  // more explicit. Alternatively the PlayerShipData could group the target
  // and the list of targets under a std::optional<Patrol> struct.
  if (data.reachedTarget)
  {
    debug("Registering AI for ship " + str(data.dbId));

    DataContext context(*data.reachedTarget);
    coordinator.addAi(shipEntity, generateBehaviorTree(data), std::move(context));
  }

  const auto maybePlayerEntityId = entityMapper.tryGetPlayerEntityId(data.playerDbId);
  if (!maybePlayerEntityId)
  {
    error("Failed to register owner for ship " + str(data.dbId),
          "Could not find entity id for player " + str(data.playerDbId));
  }

  entityMapper.registerShipForPlayer(data.playerDbId, data.dbId, shipEntity);
  coordinator.addOwner(shipEntity, *maybePlayerEntityId, OwnerType::PLAYER);
}

void ShipDataSource::registerShipWeapons(Coordinator &coordinator,
                                         const PlayerShipData &data,
                                         const Uuid shipEntity) const
{
  for (const auto &weapon : data.weapons)
  {
    coordinator.addWeapon(shipEntity, weapon);
  }
}

void ShipDataSource::registerShipComputers(Coordinator &coordinator,
                                           const PlayerShipData &data,
                                           const Uuid shipEntity) const
{
  for (const auto &computer : data.computers)
  {
    coordinator.addComputer(shipEntity, computer);
  }
}

namespace {
auto generateBehaviorNode(INodePtr &&idleSequence) -> INodePtr
{
  auto fire         = std::make_unique<FireNode>();
  auto followTarget = std::make_unique<FollowTargetNode>();
  auto pickTarget   = std::make_unique<PickTargetNode>();

  auto fireAndFollowFallback = std::make_unique<FallbackNode>();
  fireAndFollowFallback->addChild(std::move(fire));
  fireAndFollowFallback->addChild(std::move(followTarget));

  auto attackSequence = std::make_unique<SequenceNode>();
  attackSequence->addChild(std::move(pickTarget));
  attackSequence->addChild(std::move(fireAndFollowFallback));

  auto fallbackNode = std::make_unique<FallbackNode>();
  fallbackNode->addChild(std::move(attackSequence));
  fallbackNode->addChild(std::move(idleSequence));

  auto repeater = std::make_unique<RepeaterNode>(std::move(fallbackNode));

  return repeater;
}
} // namespace

auto ShipDataSource::generateBehaviorTree(const PlayerShipData &data) const -> INodePtr
{
  auto idleSequence = std::make_unique<SequenceNode>();

  for (std::size_t id = 0u; id < data.aiTargets.size(); ++id)
  {
    auto targetNode = std::make_unique<TargetNode>(data.aiTargets[id], static_cast<int>(id));
    debug("Picked target " + str(data.aiTargets[id]) + " for " + str(data.dbId));
    idleSequence->addChild(std::move(targetNode));
  }

  return generateBehaviorNode(std::move(idleSequence));
}

} // namespace bsgo
