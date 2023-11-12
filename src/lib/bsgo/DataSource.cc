
#include "DataSource.hh"
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

#include <iostream>

namespace bsgo {

DataSource::DataSource()
  : utils::CoreObject("bsgo")
  , m_repositories(createRepositories(m_connection))
{
  setService("data");
  m_connection->connect();
}

auto DataSource::repositories() const -> Repositories
{
  return m_repositories;
}

auto DataSource::playerId() const -> Uuid
{
  return m_playerId;
}

auto DataSource::playerEntityId() const -> Uuid
{
  if (!m_playerEntityId)
  {
    error("Expected to have a player entity id");
  }
  return *m_playerEntityId;
}

auto DataSource::playerShipId() const -> Uuid
{
  if (!m_playerShipId)
  {
    error("Expected to have a player ship id");
  }
  return *m_playerShipId;
}

auto DataSource::playerShipEntityId() const -> Uuid
{
  if (!m_playerShipEntityId)
  {
    error("Expected to have a player ship entity id");
  }
  return *m_playerShipEntityId;
}

void DataSource::initialize(Coordinator &coordinator) const
{
  const auto systemId = m_repositories.playerRepository->findSystemById(m_playerId);

  initializePlayer(coordinator);
  initializeShips(coordinator, systemId);
  initializeAsteroids(coordinator, systemId);
  initializeOutposts(coordinator, systemId);
}

void DataSource::initializePlayer(Coordinator &coordinator) const
{
  m_playerEntityId = coordinator.createEntity(EntityKind::PLAYER);

  const auto resources = m_repositories.playerResourceRepository->findAllByPlayer(m_playerId);
  for (const auto &resource : resources)
  {
    const auto data = m_repositories.playerResourceRepository->findOneById(resource);
    coordinator.addResourceComponent(*m_playerEntityId, data.resource, data.amount);
  }
}

void DataSource::initializeAsteroids(Coordinator &coordinator, const Uuid &system) const
{
  const auto asteroids = m_repositories.systemRepository->findAllAsteroidsBySystem(system);
  for (const auto &id : asteroids)
  {
    registerAsteroid(coordinator, id);
  }
}

void DataSource::initializeShips(Coordinator &coordinator, const Uuid &system) const
{
  const auto ships = m_repositories.systemRepository->findAllShipsBySystem(system);
  for (const auto &id : ships)
  {
    registerShip(coordinator, id);
  }
}

void DataSource::initializeOutposts(Coordinator &coordinator, const Uuid &system) const
{
  const auto outposts = m_repositories.systemRepository->findAllOutpostsBySystem(system);
  for (const auto &id : outposts)
  {
    registerOutpost(coordinator, id);
  }
}

void DataSource::registerAsteroid(Coordinator &coordinator, const Uuid &asteroid) const
{
  const auto data = m_repositories.asteroidRepository->findOneById(asteroid);

  auto box       = std::make_unique<CircleBox>(data.position, data.radius);
  const auto ent = coordinator.createEntity(EntityKind::ASTEROID);
  coordinator.addTransform(ent, std::move(box));
  coordinator.addHealth(ent, data.health, data.health, 0.0f);
  coordinator.addRemoval(ent);
  coordinator.addScanned(ent);
  if (data.loot)
  {
    coordinator.addLoot(ent);
    const auto loot = m_repositories.asteroidLootRepository->findOneById(asteroid);
    coordinator.addResourceComponent(ent, loot.resource, loot.amount);
  }
}

void DataSource::registerShip(Coordinator &coordinator, const Uuid &ship) const
{
  const auto data = m_repositories.playerShipRepository->findOneById(ship);

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
  coordinator.addStatus(ent, Status::VISIBLE);
  if (!data.player)
  {
    coordinator.addAI(ent, generateBehaviorTree(ship, data.position));
  }
  else if (*data.player == m_playerId)
  {
    if (!m_playerEntityId)
    {
      error("Failed to create ship " + str(ship), "Expected player entity to be created already");
    }
    if (m_playerShipId)
    {
      error("Failed to create ship " + str(ship),
            "Player ship id is already assigned entity " + str(*m_playerShipId));
    }
    if (m_playerShipEntityId)
    {
      error("Failed to create ship " + str(ship),
            "Player ship entity id is already assigned entity " + str(*m_playerShipEntityId));
    }

    m_playerShipId       = ship;
    m_playerShipEntityId = ent;
    coordinator.addOwner(ent, *m_playerEntityId, OwnerType::PLAYER);
  }

  registerShipWeapons(coordinator, ship, ent);
  registerShipComputers(coordinator, ship, ent);
}

void DataSource::registerOutpost(Coordinator &coordinator, const Uuid &outpost) const
{
  const auto data = m_repositories.systemOutpostRepository->findOneById(outpost);

  auto box       = std::make_unique<CircleBox>(data.position, data.radius);
  const auto ent = coordinator.createEntity(EntityKind::OUTPOST);
  coordinator.addTransform(ent, std::move(box));
  coordinator.addHealth(ent, data.hullPoints, data.maxHullPoints, data.hullPointsRegen);
  coordinator.addRemoval(ent);
  coordinator.addPower(ent, data.powerPoints, data.maxPowerPoints, data.powerRegen);
  coordinator.addTarget(ent);
  coordinator.addFaction(ent, data.faction);
}

void DataSource::registerShipWeapons(Coordinator &coordinator,
                                     const Uuid &ship,
                                     const Uuid &shipEntity) const
{
  const auto weapons = m_repositories.shipWeaponRepository->findAllByShip(ship);
  for (const auto &weapon : weapons)
  {
    const auto data = m_repositories.playerWeaponRepository->findOneById(weapon);
    coordinator.addWeapon(shipEntity, data);
  }
}

void DataSource::registerShipComputers(Coordinator &coordinator,
                                       const Uuid &ship,
                                       const Uuid &shipEntity) const
{
  const auto computers = m_repositories.shipComputerRepository->findAllByShip(ship);
  for (const auto &computer : computers)
  {
    const auto data = m_repositories.playerComputerRepository->findOneById(computer);
    coordinator.addComputer(shipEntity, data);
  }
}

auto DataSource::generateBehaviorTree(const Uuid &entity, const Eigen::Vector3f &center) const
  -> INodePtr
{
  constexpr auto RADIUS_TO_PICK_A_TARGET = 5.0f;
  utils::RNG rng(entity);

  auto dx                       = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  auto dy                       = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  const Eigen::Vector3f target1 = center + Eigen::Vector3f(dx, dy, 0.0f);
  log("Picked first target " + str(target1) + " for " + str(entity));

  dx                            = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  dy                            = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  const Eigen::Vector3f target2 = center + Eigen::Vector3f(dx, dy, 0.0f);
  log("Picked second target " + str(target2) + " for " + str(entity));

  dx                            = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  dy                            = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  const Eigen::Vector3f target3 = center + Eigen::Vector3f(dx, dy, 0.0f);
  log("Picked third target " + str(target3) + " for " + str(entity));

  dx                            = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  dy                            = rng.rndFloat(-RADIUS_TO_PICK_A_TARGET, RADIUS_TO_PICK_A_TARGET);
  const Eigen::Vector3f target4 = center + Eigen::Vector3f(dx, dy, 0.0f);
  log("Picked fourth target " + str(target4) + " for " + str(entity));

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
