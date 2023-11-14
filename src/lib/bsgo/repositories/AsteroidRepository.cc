
#include "AsteroidRepository.hh"

namespace bsgo {

AsteroidRepository::AsteroidRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("asteroid", connection)
{}

namespace {
constexpr auto SQL_QUERY = "SELECT health, radius, x_pos, y_pos, z_pos FROM asteroid WHERE id = ";
auto generateSqlQuery(const Uuid &asteroid) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(asteroid));
}
} // namespace

auto AsteroidRepository::findOneById(const Uuid &asteroid) const -> Asteroid
{
  const auto sql = generateSqlQuery(asteroid);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one asteroid with id " + str(asteroid));
  }

  Asteroid out;

  const auto &record = rows[0];
  out.health         = record[0].as<float>();
  out.radius         = record[1].as<float>();

  const auto x = record[2].as<float>();
  const auto y = record[3].as<float>();
  const auto z = record[4].as<float>();
  out.position = Eigen::Vector3f(x, y, z);

  return out;
}

} // namespace bsgo
