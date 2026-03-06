
#pragma once

#include "DbConnection.hh"
#include "IRepository.hh"

namespace bsgalone::core {

class AbstractRepository : public IRepository
{
  public:
  AbstractRepository(const std::string &name, const DbConnectionShPtr &connection);
  ~AbstractRepository() override = default;

  protected:
  DbConnectionShPtr m_connection;
};

} // namespace bsgalone::core
