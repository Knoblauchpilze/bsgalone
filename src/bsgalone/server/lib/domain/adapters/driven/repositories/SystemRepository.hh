
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

  auto findOneById(const core::Uuid system) const -> System override;
  auto findAll() const -> std::vector<System> override;

  void save(const System &system) override;
};

using SystemRepositoryShPtr = std::shared_ptr<SystemRepository>;

} // namespace bsgalone::server
