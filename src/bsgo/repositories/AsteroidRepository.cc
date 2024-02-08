
#include "AsteroidRepository.hh"

namespace bsgo {

AsteroidRepository::AsteroidRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("asteroid", connection)
{}

namespace {
constexpr auto FIND_ONE_QUERY_NAME = "asteroid_find_one";
constexpr auto FIND_ONE_QUERY
  = "SELECT system, health, radius, x_pos, y_pos, z_pos FROM asteroid WHERE id = $1";

constexpr auto FIND_LOOT_QUERY_NAME = "asteroid_find_loot";
constexpr auto FIND_LOOT_QUERY = "SELECT count(resource) FROM asteroid_loot WHERE asteroid = $1";
} // namespace

void AsteroidRepository::initialize()
{
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_LOOT_QUERY_NAME, FIND_LOOT_QUERY);
}

auto AsteroidRepository::findOneById(const Uuid asteroid) const -> Asteroid
{
  auto out = fetchAsteroidBase(asteroid);
  fetchLoot(asteroid, out);

  return out;
}

auto AsteroidRepository::fetchAsteroidBase(const Uuid asteroid) const -> Asteroid
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(asteroid));

  Asteroid out;

  out.system = fromDbId(record[0].as<int>());
  out.health = record[1].as<float>();
  out.radius = record[2].as<float>();

  const auto x = record[3].as<float>();
  const auto y = record[4].as<float>();
  const auto z = record[5].as<float>();
  out.position = Eigen::Vector3f(x, y, z);

  return out;
}

void AsteroidRepository::fetchLoot(const Uuid asteroid, Asteroid &out) const
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_LOOT_QUERY_NAME, toDbId(asteroid));

  out.loot = record[0].as<int>() > 0;
}

} // namespace bsgo
