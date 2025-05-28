
#pragma once

#include "CoreObject.hh"
#include "DatabaseEntityMapper.hh"
#include "Repositories.hh"

namespace bsgo {

class Coordinator;

class AsteroidDataSource : public core::CoreObject
{
  public:
  AsteroidDataSource(const Repositories &repositories);
  ~AsteroidDataSource() override = default;

  void initialize(const Uuid systemDbId,
                  Coordinator &coordinator,
                  DatabaseEntityMapper &entityMapper) const;

  void registerAsteroid(Coordinator &coordinator,
                        const Uuid asteroidDbId,
                        DatabaseEntityMapper &entityMapper) const;

  private:
  Repositories m_repositories{};
};

} // namespace bsgo
