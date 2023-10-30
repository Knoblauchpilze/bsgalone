
#include "BulletSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return EntityKind::BULLET == entity.kind->kind();
}
} // namespace

BulletSystem::BulletSystem()
  : AbstractSystem("bullet", isEntityRelevant)
{}

void BulletSystem::updateEntity(Entity & /*entity*/,
                                Coordinator & /*coordinator*/,
                                const float /*elapsedSeconds*/) const
{}

} // namespace bsgo
