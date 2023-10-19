
#pragma once

#include "Components.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class Coordinator;

class ISystem : public utils::CoreObject
{
  public:
  ISystem(const std::string &name);
  virtual ~ISystem() = default;

  virtual void update(const Coordinator &coordinator, const float elapsedSeconds) = 0;
};

using ISystemPtr = std::unique_ptr<ISystem>;

} // namespace bsgo
