
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class IComponent : public utils::CoreObject
{
  public:
  IComponent(const std::string &name);
  ~IComponent() override = default;

  virtual void update(const float elapsedSeconds) = 0;
};

using IComponentShPtr = std::shared_ptr<IComponent>;

} // namespace bsgo
