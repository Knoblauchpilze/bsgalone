
#pragma once

#include "System.hh"
#include <memory>

namespace bsgalone::core {

class ForFetchingSystem
{
  public:
  ForFetchingSystem()          = default;
  virtual ~ForFetchingSystem() = default;

  virtual auto findOneById(const Uuid systemDbId) const -> System = 0;
};

using ForFetchingSystemShPtr = std::shared_ptr<ForFetchingSystem>;

} // namespace bsgalone::core
