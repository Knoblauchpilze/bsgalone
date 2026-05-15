
#include "TestDataFactory.hh"

namespace test {

auto generateAsteroid() -> bsgalone::core::Asteroid
{
  return bsgalone::core::Asteroid{
    .dbId      = bsgalone::core::Uuid{},
    .position  = Eigen::Vector3f(1.0f, -3.0f, 9.5f),
    .radius    = 1.78f,
    .health    = 17.8f,
    .maxHealth = 58.2f,
  };
}

} // namespace test
