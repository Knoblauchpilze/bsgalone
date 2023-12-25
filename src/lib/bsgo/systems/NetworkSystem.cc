
#include "NetworkSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<NetworkComponent>();
}
} // namespace

NetworkSystem::NetworkSystem()
  : AbstractSystem("network", isEntityRelevant)
{}

void NetworkSystem::updateEntity(Entity &entity,
                                 Coordinator & /*coordinator*/,
                                 const float /*elapsedSeconds*/) const
{
  const auto networkComp = entity.networkComp();
  if (!networkComp.needsSync())
  {
    return;
  }

  syncEntity(entity);
}

void NetworkSystem::syncEntity(const Entity &entity) const
{
  warn("Should sync entity " + entity.str());
}

} // namespace bsgo
