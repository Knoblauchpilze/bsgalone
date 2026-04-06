
#include "AbstractRepository.hh"

namespace bsgalone::server {

AbstractRepository::AbstractRepository(const std::string &name, DbConnectionShPtr connection)
  : ::core::CoreObject(name)
  , m_connection(std::move(connection))
{
  setService("repository");

  if (m_connection == nullptr)
  {
    throw std::invalid_argument("Expected non null connection");
  }
}

} // namespace bsgalone::server
