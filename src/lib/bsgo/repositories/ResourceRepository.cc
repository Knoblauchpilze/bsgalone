
#include "ResourceRepository.hh"

namespace bsgo {

ResourceRepository::ResourceRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("resource", connection)
{}

namespace {
constexpr auto FIND_ONE_QUERY_NAME = "resource_find_one";
constexpr auto FIND_ONE_QUERY      = "SELECT name FROM resource WHERE id = $1";
} // namespace

void ResourceRepository::initialize()
{
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
}

auto ResourceRepository::findOneById(const Uuid &resource) const -> Resource
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(resource));

  Resource out;

  out.id   = resource;
  out.name = record[0].as<std::string>();

  return out;
}

} // namespace bsgo
