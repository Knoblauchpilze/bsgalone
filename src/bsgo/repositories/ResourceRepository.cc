
#include "ResourceRepository.hh"

namespace bsgo {

ResourceRepository::ResourceRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("resource", connection)
{}

namespace {
constexpr auto FIND_ONE_QUERY_NAME = "resource_find_one";
constexpr auto FIND_ONE_QUERY      = "SELECT name FROM resource WHERE id = $1";

constexpr auto FIND_ONE_BY_NAME_QUERY_NAME = "player_find_one_by_name";
constexpr auto FIND_ONE_BY_NAME_QUERY      = "SELECT id FROM resource WHERE name = $1";

} // namespace

void ResourceRepository::initialize()
{
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ONE_BY_NAME_QUERY_NAME, FIND_ONE_BY_NAME_QUERY);
}

auto ResourceRepository::findOneById(const Uuid resource) const -> Resource
{
  const auto query = [resource](pqxx::nontransaction &work) {
    return work.exec(FIND_ONE_QUERY_NAME, pqxx::params{toDbId(resource)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  Resource out;

  out.id   = resource;
  out.name = record[0].as<std::string>();

  return out;
}

auto ResourceRepository::findOneByName(const std::string &name) const -> std::optional<Resource>
{
  const auto query = [name](pqxx::nontransaction &work) {
    return work.exec(FIND_ONE_BY_NAME_QUERY_NAME, pqxx::params{name});
  };
  const auto rows = m_connection->executeQuery(query);

  if (rows.empty())
  {
    return {};
  }

  if (rows.size() != 1)
  {
    error("Expected to find only one resource with name \"" + name + "\"");
  }

  Resource out;

  const auto &record = rows[0];
  out.id             = fromDbId(record[0].as<int>());
  out.name           = name;

  return out;
}

} // namespace bsgo
