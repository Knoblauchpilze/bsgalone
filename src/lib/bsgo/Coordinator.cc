
#include "Coordinator.hh"
#include "CircleBox.hh"

namespace bsgo {

Coordinator::Coordinator(const Repositories &repos)
  : utils::CoreObject("coordinator")
{
  setService("bsgo");
  init(repos);
}

auto Coordinator::createEntity(const EntityKind &kind) -> Uuid
{
  auto ent        = m_entities.size();
  m_entities[ent] = kind;
  return ent;
}

auto Coordinator::createEntity(const EntityKind &kind, IBoundingBoxPtr bbox) -> Uuid
{
  const auto ent = createEntity(kind);
  addTransform(ent, std::move(bbox));
  return ent;
}

auto Coordinator::createEntity(const EntityKind &kind,
                               IBoundingBoxPtr bbox,
                               const Eigen::Vector3f &speed) -> Uuid
{
  const auto ent = createEntity(kind);
  addTransform(ent, std::move(bbox));
  addVelocity(ent, speed);
  return ent;
}

auto Coordinator::getEntity(const Uuid &ent) const -> Entity
{
  Entity out;
  out.uuid = ent;

  const auto it = m_entities.find(ent);
  if (it != m_entities.end())
  {
    out.kind = it->second;
  }

  out.transform = getTransform(ent);
  out.velocity  = getVelocity(ent);

  return out;
}

auto Coordinator::getTransform(const Uuid &ent) const -> std::optional<TransformShPtr>
{
  const auto it = m_components.transforms.find(ent);
  if (m_components.transforms.end() == it)
  {
    return {};
  }

  return {it->second};
}

auto Coordinator::getVelocity(const Uuid &ent) const -> std::optional<VelocityShPtr>
{
  const auto it = m_components.velocities.find(ent);
  if (m_components.velocities.end() == it)
  {
    return {};
  }

  return {it->second};
}

auto Coordinator::getEntityAt(const Eigen::Vector3f &pos,
                              const std::optional<EntityKind> &filter) const -> std::optional<Uuid>
{
  std::optional<Uuid> out;
  float best = std::numeric_limits<float>::max();

  for (const auto &[uuid, transform] : m_components.transforms)
  {
    if (hasExpectedKind(uuid, filter) && transform->contains(pos))
    {
      const auto d = (pos - transform->position()).norm();
      if (d < best)
      {
        out  = uuid;
        best = d;
      }
    }
  }

  return out;
}

auto Coordinator::getEntitiesWithin(const IBoundingBox &bbox,
                                    const std::optional<EntityKind> &filter) const
  -> std::vector<Uuid>
{
  std::vector<Uuid> out;

  for (const auto &[uuid, transform] : m_components.transforms)
  {
    /// TODO: We should probably have a 'intersects' method.
    if (hasExpectedKind(uuid, filter) && bbox.isInside(transform->position()))
    {
      out.push_back(uuid);
    }
  }

  return out;
}

void Coordinator::update(float elapsedSeconds)
{
  m_motionSystem.update(m_components, elapsedSeconds);
}

void Coordinator::checkRepositories(const Repositories &repos)
{
  if (nullptr == repos.system)
  {
    throw std::invalid_argument("Expected non null system repository");
  }
  if (nullptr == repos.asteroid)
  {
    throw std::invalid_argument("Expected non null asteroid repository");
  }
  if (nullptr == repos.player)
  {
    throw std::invalid_argument("Expected non null player repository");
  }
  if (nullptr == repos.ship)
  {
    throw std::invalid_argument("Expected non null ship repository");
  }
}

void Coordinator::init(const Repositories &repos)
{
  checkRepositories(repos);

  const auto systemId = repos.player->findSystemById({});

  const auto ships = repos.system->findShips(systemId);
  for (const auto &systemShip : ships)
  {
    const auto ship = repos.ship->findOneById(systemShip.model);
    const auto pos  = repos.ship->findPositionById(systemShip.ship);

    auto box = std::make_unique<CircleBox>(pos, ship.radius);
    createEntity(EntityKind::SHIP, std::move(box), Eigen::Vector3f::Zero());
  }

  const auto asteroids = repos.system->findAsteroids(systemId);
  for (const auto &id : asteroids)
  {
    const auto asteroid = repos.asteroid->findOneById(id);

    auto box = std::make_unique<CircleBox>(asteroid.position, asteroid.radius);
    createEntity(EntityKind::ASTEROID, std::move(box));
  }
}

bool Coordinator::hasExpectedKind(const Uuid &ent, const std::optional<EntityKind> &kind) const
{
  if (!kind)
  {
    return true;
  }

  const auto it = m_entities.find(ent);
  return it != m_entities.end() && it->second == *kind;
}

void Coordinator::addTransform(const Uuid &ent, IBoundingBoxPtr bbox)
{
  if (m_components.transforms.contains(ent))
  {
    warn("Overriding transform for entity " + std::to_string(ent));
  }

  m_components.transforms[ent] = std::make_shared<Transform>(std::move(bbox));
}

void Coordinator::addVelocity(const Uuid &ent, const Eigen::Vector3f &speed)
{
  if (m_components.velocities.contains(ent))
  {
    warn("Overriding velocity for entity " + std::to_string(ent));
  }

  m_components.velocities[ent] = std::make_shared<Velocity>(Eigen::Vector3f::Zero(), speed);
}

} // namespace bsgo
