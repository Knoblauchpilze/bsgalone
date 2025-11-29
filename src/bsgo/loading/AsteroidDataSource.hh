
#pragma once

#include "AsteroidData.hh"
#include "CoreObject.hh"
#include "DatabaseEntityMapper.hh"
#include "Repositories.hh"
#include <optional>

namespace bsgo {

class Coordinator;

class AsteroidDataSource : public core::CoreObject
{
  public:
  AsteroidDataSource();
  AsteroidDataSource(const Repositories &repositories);
  ~AsteroidDataSource() override = default;

  void initialize(const Uuid systemDbId,
                  Coordinator &coordinator,
                  DatabaseEntityMapper &entityMapper) const;

  void registerAsteroid(Coordinator &coordinator,
                        const AsteroidData &data,
                        DatabaseEntityMapper &entityMapper) const;

  // Thid variant is currently used by the server when an asteroid
  // respawns. We could make it to also use the overload above but
  // it's not a priority.
  void registerAsteroid(Coordinator &coordinator,
                        const Uuid asteroidDbId,
                        DatabaseEntityMapper &entityMapper) const;

  private:
  std::optional<Repositories> m_repositories{};
};

} // namespace bsgo
