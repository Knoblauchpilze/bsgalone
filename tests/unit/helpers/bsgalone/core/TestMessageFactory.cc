
#include "TestMessageFactory.hh"
#include "SystemDataMessageBuilder.hh"
#include "TestDataFactory.hh"

namespace test {

auto generateSystemDataMessage(std::vector<bsgalone::core::Asteroid> asteroids)
  -> bsgalone::core::SystemDataMessage
{
  const auto system = generateSystem({});
  return bsgalone::core::SystemDataMessageBuilder(bsgalone::core::Uuid{})
    .withSystemName(system.name)
    .withCurrentTick(system.currentTick)
    .withTimeStep(system.step)
    .withAsteroids(std::move(asteroids))
    .build();
}

} // namespace test
