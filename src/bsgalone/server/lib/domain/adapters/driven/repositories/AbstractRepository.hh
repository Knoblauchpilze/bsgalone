
#pragma once

#include "CoreObject.hh"
#include "DbConnection.hh"
#include "IRepository.hh"

namespace bsgalone::core {

class AbstractRepository : public IRepository, public ::core::CoreObject
{
  public:
  AbstractRepository(const std::string &name, DbConnectionShPtr connection);
  ~AbstractRepository() override = default;

  protected:
  DbConnectionShPtr m_connection{};
};

} // namespace bsgalone::core
