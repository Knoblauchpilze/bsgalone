
#pragma once

#include "DatabaseEntityMapper.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class Coordinator;

class AsteroidDataSource : public utils::CoreObject
{
  public:
  AsteroidDataSource(const Repositories &repositories);
  ~AsteroidDataSource() override = default;

  void initialize(const Uuid systemDbId,
                  Coordinator &coordinator,
                  DatabaseEntityMapper &entityMapper) const;

  private:
  Repositories m_repositories{};

  void registerAsteroid(Coordinator &coordinator,
                        const Uuid asteroidDbId,
                        DatabaseEntityMapper &entityMapper) const;
};

} // namespace bsgo
