
#include "ResourceRepository.hh"

namespace bsgo {

ResourceRepository::ResourceRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("resource", connection)
{}

namespace {
constexpr auto SQL_QUERY = "SELECT name FROM resource WHERE id = ";

auto generateSqlQuery(const Uuid &resource) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(resource));
}
} // namespace

auto ResourceRepository::findOneById(const Uuid &resource) const -> Resource
{
  const auto sql = generateSqlQuery(resource);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find resource " + str(resource));
  }

  Resource out;

  const auto &record = rows[0];
  out.name           = record[0].as<std::string>();

  return out;
}

} // namespace bsgo
