
#pragma once

#include "Repositories.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class Coordinator;

class AsteroidDataSource : public utils::CoreObject
{
  public:
  AsteroidDataSource(const Repositories &repositories, const Uuid &playerDbId);
  ~AsteroidDataSource() override = default;

  void initialize(Coordinator &coordinator) const;

  private:
  Uuid m_playerDbId{};
  Repositories m_repositories{};

  void registerAsteroid(Coordinator &coordinator, const Uuid &asteroid) const;
};

} // namespace bsgo
