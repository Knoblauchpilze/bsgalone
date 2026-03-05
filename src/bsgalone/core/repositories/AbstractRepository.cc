
#include "AbstractRepository.hh"

namespace bsgo {

AbstractRepository::AbstractRepository(const std::string &name, const DbConnectionShPtr &connection)
  : IRepository(name)
  , m_connection(connection)
{}

} // namespace bsgo
