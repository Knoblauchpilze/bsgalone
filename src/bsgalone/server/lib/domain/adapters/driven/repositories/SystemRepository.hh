
#pragma once

#include "AbstractRepository.hh"
#include "ForManagingSystem.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgalone::server {

class SystemRepository : public ForManagingSystem, public AbstractRepository
{
  public:
  SystemRepository(const DbConnectionShPtr &connection);
  ~SystemRepository() override = default;

  void initialize() override;

  /// @brief - Tries to fetch the system matching the provided identifier.
  /// In case no such system exists, an error is raised.
  /// @param systemDbId - the identifier of the system to fetch
  /// @return - the system matching the provided identifier
  auto findOneById(const core::Uuid systemDbId) const -> System override;

  auto findAll() const -> std::vector<System> override;

  /// @brief - Persists the system's data to the database. In case the system
  /// does not yet exist, an entry will be created for it. In case a system
  /// with the same identifier exists, it will update it's tick, tick config
  /// and position.
  /// @param system - the system to persist
  void save(const System &system) override;
};

using SystemRepositoryShPtr = std::shared_ptr<SystemRepository>;

} // namespace bsgalone::server
