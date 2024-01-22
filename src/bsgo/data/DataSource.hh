
#pragma once

#include "DbConnection.hh"
#include "INode.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class Coordinator;

class DataSource : public utils::CoreObject
{
  public:
  DataSource();
  ~DataSource() override = default;

  void setSystemDbId(const Uuid &system);
  void setPlayerDbId(const Uuid &player);
  void clearSystemDbId();

  auto repositories() const -> Repositories;
  auto playerDbId() const -> std::optional<Uuid>;
  auto playerEntityId() const -> Uuid;
  auto playerShipDbId() const -> Uuid;
  auto playerShipEntityId() const -> Uuid;

  void initialize(Coordinator &coordinator) const;

  private:
  mutable std::optional<Uuid> m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};
  mutable std::optional<Uuid> m_playerEntityId{};
  mutable std::optional<Uuid> m_playerShipDbId{};
  mutable std::optional<Uuid> m_playerShipEntityId{};

  DbConnectionShPtr m_connection{std::make_shared<DbConnection>()};
  Repositories m_repositories{};

  void initializePlayer(Coordinator &coordinator) const;
  void initializeAsteroids(Coordinator &coordinator) const;
  void initializeShips(Coordinator &coordinator) const;
  void initializeOutposts(Coordinator &coordinator) const;
};

} // namespace bsgo
