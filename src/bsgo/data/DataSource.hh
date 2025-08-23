
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

  // TODO: What about the loading of weapons/computers? Two options:
  // 1. create a getAllWeapons/registerWeapons -> those would be called by the consumer
  //   when handling the PlayerListMessage and WeaponListMessage, etc.
  // 2. add the views to the consumer and directly fill in the data there
  // Solution 2 seems better.
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
