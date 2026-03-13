
#pragma once

#include "System.hh"
#include <memory>

namespace bsgalone::core {

class ForManagingSystem
{
  public:
  ForManagingSystem()          = default;
  virtual ~ForManagingSystem() = default;

  virtual auto findOneById(const Uuid systemDbId) const -> System = 0;
  virtual void save(const System &system)                         = 0;
};

using ForManagingSystemShPtr = std::shared_ptr<ForManagingSystem>;

} // namespace bsgalone::core
