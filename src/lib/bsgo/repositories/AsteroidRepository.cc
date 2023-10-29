
#include "AsteroidRepository.hh"

namespace bsgo {

AsteroidRepository::AsteroidRepository()
  : IRepository("asteroid")
{}

auto AsteroidRepository::findOneById(const Uuid &asteroid) const -> Asteroid
{
  constexpr auto SMALL_ASTEROID_RADIUS = 0.5f;
  constexpr auto BIG_ASTEROID_RADIUS   = 1.0f;
  Asteroid out;

  switch (asteroid)
  {
    case 0:
      out.position = Eigen::Vector3f(1.0f, 2.0f, 0.0f);
      out.radius   = SMALL_ASTEROID_RADIUS;
      out.health   = 85.0f;
      out.loot     = true;
      break;
    case 1:
      out.position = Eigen::Vector3f(4.0f, 2.0f, 0.0f);
      out.radius   = BIG_ASTEROID_RADIUS;
      out.health   = 222.0f;
      break;
    case 2:
      out.position = Eigen::Vector3f(-2.0f, -6.0f, 0.0f);
      out.radius   = SMALL_ASTEROID_RADIUS;
      out.health   = 150.0f;
      out.loot     = true;
      break;
    case 3:
      out.position = Eigen::Vector3f(-3.0f, -4.0f, 0.0f);
      out.radius   = BIG_ASTEROID_RADIUS;
      out.health   = 3333.0f;
      break;
    case 4:
      out.position = Eigen::Vector3f(15.0f, 17.0f, 0.0f);
      out.radius   = BIG_ASTEROID_RADIUS;
      out.health   = 2345.0f;
      break;
    default:
      error("Asteroid " + str(asteroid) + " not found");
      break;
  }

  return out;
}

} // namespace bsgo
