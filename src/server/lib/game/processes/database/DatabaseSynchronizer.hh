
#pragma once

#include "CoreObject.hh"
#include "Entity.hh"
#include "Repositories.hh"

namespace bsgo {

class DatabaseSynchronizer : public core::CoreObject
{
  public:
  DatabaseSynchronizer(const Repositories &repositories);

  void syncComponent(const Entity &entity, const ComponentType type) const;

  private:
  Repositories m_repositories{};
};

} // namespace bsgo
