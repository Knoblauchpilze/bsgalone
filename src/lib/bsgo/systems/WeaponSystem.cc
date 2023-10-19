
#include "WeaponSystem.hh"

namespace bsgo {

WeaponSystem::WeaponSystem()
  : ISystem("weapon")
{}

void WeaponSystem::update(const Components & /*components*/,
                          const Coordinator & /*coordinator*/,
                          const float /*elapsedSeconds*/)
{
  /// https://gamedev.stackexchange.com/questions/71711/ecs-how-to-access-multiple-components-not-the-same-one-in-a-system
  /// It doesn't seem needed at the moment to have a mechanism to filter entities which
  /// have only the expected component: we can just iterate over all of them and if it
  /// becomes too slow we can change this.
}

} // namespace bsgo
