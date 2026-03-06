
#pragma once

#include "CoreObject.hh"
#include "Entity.hh"
#include "Repositories.hh"

namespace bsgalone::server {

class DatabaseSynchronizer : public ::core::CoreObject
{
  public:
  DatabaseSynchronizer(const core::Repositories &repositories);

  void syncEntity(core::Entity &entity) const;

  private:
  core::Repositories m_repositories{};
};

} // namespace bsgalone::server
