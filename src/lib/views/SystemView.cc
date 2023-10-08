
#include "SystemView.hh"
#include "CircleBox.hh"

namespace bsgo {

SystemView::SystemView(const Repositories &repositories)
  : utils::CoreObject("system")
{
  setService("view");
  if (nullptr == repositories.system)
  {
    throw std::invalid_argument("Expected non null system repository");
  }
  if (nullptr == repositories.asteroid)
  {
    throw std::invalid_argument("Expected non null asteroid repository");
  }
  init(repositories);
}

void SystemView::update(const float elapsedSeconds)
{
  m_coordinator.update(elapsedSeconds);
}

auto SystemView::getEntity(const Uuid &ent) const -> Entity
{
  return m_coordinator.getEntity(ent);
}

auto SystemView::getEntityAt(const Eigen::Vector3f &pos,
                             const std::optional<EntityKind> &filter) const -> std::optional<Entity>
{
  const auto ent = m_coordinator.getEntityAt(pos);
  if (!ent)
  {
    return {};
  }

  if (filter && !isValidForFilter(*ent, *filter))
  {
    return {};
  }

  return {m_coordinator.getEntity(*ent)};
}

auto SystemView::getEntitiesWithin(const IBoundingBox &bbox,
                                   const std::optional<EntityKind> &filter) const
  -> std::vector<Entity>
{
  const auto uuids = m_coordinator.getEntitiesWithin(bbox);

  std::vector<Entity> out;
  for (const auto &uuid : uuids)
  {
    if (!filter || isValidForFilter(uuid, *filter))
    {
      out.push_back(m_coordinator.getEntity(uuid));
    }
  }

  return out;
}

void SystemView::init(const Repositories &repositories)
{
  constexpr auto SHIP_RADIUS = 0.6f;
  Eigen::Vector3f pos        = Eigen::Vector3f::Zero();
  auto box                   = std::make_unique<CircleBox>(pos, SHIP_RADIUS);
  const auto ship            = m_coordinator.createEntity(EntityKind::SHIP,
                                               std::move(box),
                                               Eigen::Vector3f::Zero());
  m_ships.insert(ship);

  const auto asteroids = repositories.system->findAsteroids({});
  for (const auto &id : asteroids)
  {
    const auto asteroid = repositories.asteroid->findOneById(id);

    box      = std::make_unique<CircleBox>(asteroid.position, asteroid.radius);
    auto ent = m_coordinator.createEntity(EntityKind::ASTEROID, std::move(box));
    m_asteroids.insert(ent);
  }
}

bool SystemView::isValidForFilter(const Uuid &ent, const EntityKind &kind) const
{
  switch (kind)
  {
    case EntityKind::SHIP:
      return m_ships.contains(ent);
    case EntityKind::ASTEROID:
      return m_asteroids.contains(ent);
    default:
      warn("Unknown filter " + str(kind));
      break;
  }
  return false;
}

} // namespace bsgo
