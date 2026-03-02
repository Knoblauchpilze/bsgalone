
#pragma once

#include "ComponentType.hh"
#include "TickData.hh"
#include <memory>

namespace bsgalone::core {

class IComponent
{
  public:
  IComponent()          = default;
  virtual ~IComponent() = default;

  virtual auto type() const -> ComponentType = 0;

  virtual void update(const chrono::TickData &data) = 0;
};

using IComponentShPtr = std::shared_ptr<IComponent>;

} // namespace bsgalone::core
