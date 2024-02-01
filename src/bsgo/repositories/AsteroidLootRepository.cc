
#include "AsteroidLootRepository.hh"

namespace bsgo {

AsteroidLootRepository::AsteroidLootRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("loot", connection)
{
  addModule("asteroid");
}

namespace {
constexpr auto FIND_ONE_QUERY_NAME = "asteroid_loot_find_one";
constexpr auto FIND_ONE_QUERY = "SELECT resource, amount FROM asteroid_loot WHERE asteroid = $1";
} // namespace

void AsteroidLootRepository::initialize()
{
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
}

namespace {} // namespace

auto AsteroidLootRepository::findOneById(const Uuid asteroid) const -> AsteroidLoot
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(asteroid));

  AsteroidLoot out;

  out.resource = fromDbId(record[0].as<int>());
  out.amount   = record[1].as<float>();

  return out;
}

} // namespace bsgo
