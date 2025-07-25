
#include "ShipDataSource.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"
#include "RNG.hh"
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
    coordinator.addTarget(shipEntityId, *data.targetDbId);
  }
  else
  {
    coordinator.addTarget(shipEntityId);
  }
  coordinator.addNetwork(shipEntityId,
                         {ComponentType::HEALTH,
                          ComponentType::POWER,
                          ComponentType::STATUS,
                          ComponentType::TRANSFORM,
                          ComponentType::VELOCITY});

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
  const auto data = m_repositories->playerShipRepository->findOneById(shipDbId);

  PlayerShipData out{
    .dbId             = shipDbId,
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
    .status           = determineStartingStatusForShip(data),
    .shipClass        = data.shipClass,
    .name             = data.name,
    .docked           = data.docked,
    .jumpTime         = data.jumpTime,
    .jumpTimeInThreat = data.jumpTimeInThreat,

    .targetDbId = {},
    .playerDbId = data.player,
  };

  const auto weapons = m_repositories->shipWeaponRepository->findAllByShip(shipDbId);
  for (const auto &weapon : weapons)
  {
    const auto weaponDbData = m_repositories->playerWeaponRepository->findOneById(weapon.weapon);

    WeaponData weaponData{
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

    ComputerData computerData{
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
  if (!data.playerDbId)
  {
    entityMapper.registerShip(data.dbId, shipEntity);
    coordinator.addAI(shipEntity, generateBehaviorTree(shipEntity, data.position));
    return;
  }

  const auto maybePlayerEntityId = entityMapper.tryGetPlayerEntityId(*data.playerDbId);
  if (!maybePlayerEntityId)
  {
    error("Failed to register owner for ship " + str(data.dbId),
          "Could not find entity id for player " + str(*data.playerDbId));
  }

  entityMapper.registerShipForPlayer(*data.playerDbId, data.dbId, shipEntity);
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

auto ShipDataSource::generateBehaviorTree(const Uuid entity, const Eigen::Vector3f &center) const
  -> INodePtr
{
  constexpr auto RADIUS_TO_PICK_A_TARGET = 5.0f;
  core::RNG rng(entity);

  auto dx                       = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  auto dy                       = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  const Eigen::Vector3f target1 = center + Eigen::Vector3f(dx, dy, 0.0f);
  verbose("Picked first target " + str(target1) + " for " + str(entity));

  dx                            = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  dy                            = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  const Eigen::Vector3f target2 = center + Eigen::Vector3f(dx, dy, 0.0f);
  verbose("Picked second target " + str(target2) + " for " + str(entity));

  dx                            = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  dy                            = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  const Eigen::Vector3f target3 = center + Eigen::Vector3f(dx, dy, 0.0f);
  verbose("Picked third target " + str(target3) + " for " + str(entity));

  dx                            = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  dy                            = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  const Eigen::Vector3f target4 = center + Eigen::Vector3f(dx, dy, 0.0f);
  verbose("Picked fourth target " + str(target4) + " for " + str(entity));

  auto targetNode1  = std::make_unique<TargetNode>(target1);
  auto targetNode2  = std::make_unique<TargetNode>(target2);
  auto targetNode3  = std::make_unique<TargetNode>(target3);
  auto targetNode4  = std::make_unique<TargetNode>(target4);
  auto sequenceIdle = std::make_unique<SequenceNode>();
  sequenceIdle->addChild(std::move(targetNode1));
  sequenceIdle->addChild(std::move(targetNode2));
  sequenceIdle->addChild(std::move(targetNode3));
  sequenceIdle->addChild(std::move(targetNode4));

  auto pickTarget = std::make_unique<PickTargetNode>();
  auto target     = std::make_unique<FollowTargetNode>();
  auto fire       = std::make_unique<FireNode>();

  auto sequenceAttack = std::make_unique<SequenceNode>();
  sequenceAttack->addChild(std::move(pickTarget));
  sequenceAttack->addChild(std::move(target));
  sequenceAttack->addChild(std::move(fire));

  auto fallbackNode = std::make_unique<FallbackNode>();
  fallbackNode->addChild(std::move(sequenceAttack));
  fallbackNode->addChild(std::move(sequenceIdle));

  auto repeater = std::make_unique<RepeaterNode>(std::move(fallbackNode));

  return repeater;
}

} // namespace bsgo
