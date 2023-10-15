
#pragma once

#include "Components.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class ISystem : public utils::CoreObject
{
  public:
  ISystem(const std::string &name);
  virtual ~ISystem() = default;

  virtual void update(const Components &components, const float elapsedSeconds) = 0;
};

using ISystemPtr = std::unique_ptr<ISystem>;

} // namespace bsgo
