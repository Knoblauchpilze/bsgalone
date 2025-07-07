
#pragma once

#include "CoreObject.hh"
#include "DataLoadingMode.hh"
#include "DatabaseEntityMapper.hh"
#include "Repositories.hh"

namespace bsgo {

class Coordinator;

class DataSource : public core::CoreObject
{
  public:
  DataSource(const DataLoadingMode dataLoadingMode);
  ~DataSource() override = default;

  void setSystemDbId(const Uuid system);
  void setPlayerDbId(const Uuid player);
  void clearSystemDbId();

  // TODO: In the client case, we want to not expose this method. This means checking
  // the data loading mode and throwing an exception if it's CLIENT.
  auto repositories() const -> Repositories;

  // TODO: What about the loading of weapons/computers? Two options:
  // 1. create a getAllWeapons/registerWeapons -> those would be called by the consumer
  //   when handling the PlayerListMessage and WeaponListMessage, etc.
  // 2. add the views to the consumer and directly fill in the data there
  // Solution 2 seems better.

  // TODO: This method and the private ones will only be callable in SERVER mode.
  // In CLIENT mode we will receive messages and directly populate the entity mapper
  // and coordinator in the loading/logging service.
  void initialize(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;

  private:
  // TODO: When all of the previous comments are done, we can probably remove this
  // and not instantiate the DataSource in the client at all.
  DataLoadingMode m_dataLoadingMode{};
  // TODO: This should be extracted in the case of the client.
  // We could create a GameDataModule or similar which would be
  // responsible to handle it. We can hook this module in the
  // Game in place of where the DataSource was used.
  // For the server we could make it mandatory as we expect to
  // have one data source per system.
  mutable std::optional<Uuid> m_systemDbId{};

  Repositories m_repositories{};

  void initializePlayers(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;
  void initializeAsteroids(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;
  void initializeShips(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;
  void initializeOutposts(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;
};

} // namespace bsgo
