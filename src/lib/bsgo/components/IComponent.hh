
#pragma once

#include "ComponentType.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class IComponent : public utils::CoreObject
{
  public:
  IComponent(const std::string &name);
  ~IComponent() override = default;

  virtual auto type() const -> ComponentType = 0;

  virtual void update(const float elapsedSeconds) = 0;
};

using IComponentShPtr = std::shared_ptr<IComponent>;

} // namespace bsgo
