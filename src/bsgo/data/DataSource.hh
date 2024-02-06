
#pragma once

#include "DataLoadingMode.hh"
#include "DatabaseEntityMapper.hh"
#include "DbConnection.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class Coordinator;

class DataSource : public utils::CoreObject
{
  public:
  DataSource(const DataLoadingMode dataLoadingMode);
  ~DataSource() override = default;

  void setSystemDbId(const Uuid system);
  void setPlayerDbId(const Uuid player);
  void clearSystemDbId();

  auto repositories() const -> Repositories;
  auto tryGetPlayerDbId() const -> std::optional<Uuid>;
  auto tryGetPlayerShipDbId() const -> std::optional<Uuid>;

  void initialize(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;

  private:
  DataLoadingMode m_dataLoadingMode{};
  mutable std::optional<Uuid> m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};

  DbConnectionShPtr m_connection{std::make_shared<DbConnection>()};
  Repositories m_repositories{};

  void initializePlayer(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;
  void initializeAsteroids(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;
  void initializeShips(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const;
  void initializeOutposts(Coordinator &coordinator) const;
};

} // namespace bsgo
