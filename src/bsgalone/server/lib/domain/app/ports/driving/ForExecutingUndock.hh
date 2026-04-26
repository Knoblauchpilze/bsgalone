
#pragma once

#include "UndockData.hh"
#include <memory>

namespace bsgalone::server {

class ForExecutingUndock
{
  public:
  ForExecutingUndock()          = default;
  virtual ~ForExecutingUndock() = default;

  virtual void performUndock(const UndockData &data) = 0;
};

using ForExecutingUndockPtr = std::unique_ptr<ForExecutingUndock>;

} // namespace bsgalone::server
