
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

void NetworkSystem::updateEntity(Entity & /*entity*/,
                                 Coordinator & /*coordinator*/,
                                 const float /*elapsedSeconds*/) const
{}

} // namespace bsgo
