
#pragma once

#include "Components.hh"
#include "IMessageQueue.hh"
#include "SystemType.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class Coordinator;

class ISystem : public utils::CoreObject
{
  public:
  ISystem(const std::string &name);
  ~ISystem() override = default;

  virtual auto type() const -> SystemType                       = 0;
  virtual void installMessageQueue(IMessageQueue *messageQueue) = 0;

  virtual void update(Coordinator &coordinator, const float elapsedSeconds) const = 0;
};

using ISystemPtr = std::unique_ptr<ISystem>;

} // namespace bsgo
