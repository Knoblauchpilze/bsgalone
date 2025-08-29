
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
  const auto query = [asteroid](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_QUERY_NAME}, pqxx::params{toDbId(asteroid)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  AsteroidLoot out;

  out.resource = fromDbId(record[0].as<int>());
  out.amount   = record[1].as<int>();

  return out;
}

} // namespace bsgo
