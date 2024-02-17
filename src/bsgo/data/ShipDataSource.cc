
#include "ShipDataSource.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"
#include "VectorUtils.hh"
#include <core_utils/RNG.hh>

#include "FallbackNode.hh"
#include "FireNode.hh"
#include "FollowTargetNode.hh"
#include "PickTargetNode.hh"
#include "RepeaterNode.hh"
#include "SequenceNode.hh"
#include "TargetNode.hh"

namespace bsgo {

ShipDataSource::ShipDataSource(const Repositories &repositories)
  : utils::CoreObject("bsgo")
  , m_repositories(repositories)
{
  setService("data");
  addModule("ship");
}

void ShipDataSource::initialize(const Uuid systemDbId,
                                Coordinator &coordinator,
                                DatabaseEntityMapper &entityMapper) const
{
  const auto ships = m_repositories.systemRepository->findAllShipsBySystem(systemDbId);
  for (const auto &id : ships)
  {
    registerShip(coordinator, id, entityMapper, true);
  }
}

void ShipDataSource::registerShip(Coordinator &coordinator,
                                  const Uuid ship,
                                  DatabaseEntityMapper &entityMapper) const
{
  registerShip(coordinator, ship, entityMapper, false);
}

void ShipDataSource::registerShip(Coordinator &coordinator,
                                  const Uuid ship,
                                  DatabaseEntityMapper &entityMapper,
                                  const bool ignoreDocked) const
{
  const auto data = m_repositories.playerShipRepository->findOneById(ship);

  if (ignoreDocked && data.docked)
  {
    return;
  }

  auto box       = std::make_unique<CircleBox>(data.position, data.radius);
  const auto ent = coordinator.createEntity(EntityKind::SHIP);
  coordinator.addTransform(ent, std::move(box));
  VelocityData vData{.maxAcceleration = data.acceleration, .maxSpeed = data.speed};
  coordinator.addVelocity(ent, vData);
  coordinator.addHealth(ent, data.hullPoints, data.maxHullPoints, data.hullPointsRegen);
  coordinator.addRemoval(ent);
  coordinator.addPower(ent, data.powerPoints, data.maxPowerPoints, data.powerRegen);
  coordinator.addTarget(ent);
  coordinator.addFaction(ent, data.faction);
  const auto status = data.docked ? Status::DOCKED : Status::APPEARING;
  coordinator.addStatus(ent, status, data.jumpTime, data.jumpTimeInThreat);
  coordinator.addShipClass(ent, data.shipClass);
  coordinator.addName(ent, data.name);
  coordinator.addDbId(ent, data.id);
  coordinator.addNetwork(ent,
                         {ComponentType::HEALTH,
                          ComponentType::POWER,
                          ComponentType::STATUS,
                          ComponentType::TRANSFORM,
                          ComponentType::VELOCITY});

  registerShipOwner(coordinator, ent, data, entityMapper);
  registerShipWeapons(coordinator, ship, ent);
  registerShipComputers(coordinator, ship, ent);
}

void ShipDataSource::registerShipOwner(Coordinator &coordinator,
                                       const Uuid shipEntity,
                                       const PlayerShip &shipData,
                                       DatabaseEntityMapper &entityMapper) const
{
  if (!shipData.player)
  {
    entityMapper.registerShip(shipData.id, shipEntity);
    coordinator.addAI(shipEntity, generateBehaviorTree(shipEntity, shipData.position));
    return;
  }

  const auto maybePlayerEntityId = entityMapper.tryGetPlayerEntityId(*shipData.player);
  if (!maybePlayerEntityId)
  {
    error("Failed to register owner for ship " + str(shipData.id),
          "Could not find entity id for player " + str(*shipData.player));
  }

  entityMapper.registerShipForPlayer(*shipData.player, shipData.id, shipEntity);
  coordinator.addOwner(shipEntity, *maybePlayerEntityId, OwnerType::PLAYER);
}

void ShipDataSource::registerShipWeapons(Coordinator &coordinator,
                                         const Uuid ship,
                                         const Uuid shipEntity) const
{
  const auto weapons = m_repositories.shipWeaponRepository->findAllByShip(ship);
  for (const auto &weapon : weapons)
  {
    const auto data = m_repositories.playerWeaponRepository->findOneById(weapon.weapon);
    coordinator.addWeapon(shipEntity, data, weapon.slotPosition);
  }
}

void ShipDataSource::registerShipComputers(Coordinator &coordinator,
                                           const Uuid ship,
                                           const Uuid shipEntity) const
{
  const auto computers = m_repositories.shipComputerRepository->findAllByShip(ship);
  for (const auto &computer : computers)
  {
    const auto data = m_repositories.playerComputerRepository->findOneById(computer);
    coordinator.addComputer(shipEntity, data);
  }
}

auto ShipDataSource::generateBehaviorTree(const Uuid entity, const Eigen::Vector3f &center) const
  -> INodePtr
{
  constexpr auto RADIUS_TO_PICK_A_TARGET = 5.0f;
  utils::RNG rng(entity);

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
