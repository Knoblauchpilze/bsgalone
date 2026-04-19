
#pragma once

#include "System.hh"
#include <memory>
#include <vector>

namespace bsgalone::server {

class ForManagingSystem
{
  public:
  ForManagingSystem()          = default;
  virtual ~ForManagingSystem() = default;

  virtual auto findOneById(const core::Uuid systemDbId) const -> System = 0;
  virtual auto findAll() const -> std::vector<System>                   = 0;
  virtual void save(const System &system) const                         = 0;
};

using ForManagingSystemShPtr = std::shared_ptr<ForManagingSystem>;

} // namespace bsgalone::server
