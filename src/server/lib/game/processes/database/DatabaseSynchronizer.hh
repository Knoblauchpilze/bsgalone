
#pragma once

#include "CoreObject.hh"
#include "Entity.hh"
#include "Repositories.hh"

namespace bsgo {

class DatabaseSynchronizer : public core::CoreObject
{
  public:
  DatabaseSynchronizer(const Repositories &repositories);

  void syncEntity(Entity &entity) const;

  private:
  Repositories m_repositories{};
};

} // namespace bsgo
