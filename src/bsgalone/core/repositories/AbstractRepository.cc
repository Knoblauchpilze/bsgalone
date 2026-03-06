
#include "AbstractRepository.hh"

namespace bsgalone::core {

AbstractRepository::AbstractRepository(const std::string &name, const DbConnectionShPtr &connection)
  : IRepository(name)
  , m_connection(connection)
{}

} // namespace bsgalone::core
