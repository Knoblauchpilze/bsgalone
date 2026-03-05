
#pragma once

#include "CoreObject.hh"
#include "DatabaseEntityMapper.hh"
#include "Repositories.hh"

namespace bsgo {

class Coordinator;

class DataSource : public core::CoreObject
{
  public:
  DataSource(const Uuid systemDbId);
  ~DataSource() override = default;

  void initialize(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;

  private:
  Uuid m_systemDbId{};
  Repositories m_repositories{};

  void initializePlayers(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;
  void initializeAsteroids(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;
  void initializeShips(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;
  void initializeOutposts(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;
};

} // namespace bsgo
