
#pragma once

#include "CoreObject.hh"
#include "ProcessType.hh"
#include <memory>

namespace bsgo {

class Coordinator;

class IProcess : public core::CoreObject
{
  public:
  IProcess(const std::string &name);
  ~IProcess() override = default;

  virtual auto type() const -> ProcessType = 0;

  virtual void update(Coordinator &coordinator, const float elapsedSeconds) const = 0;
};

using IProcessPtr = std::unique_ptr<IProcess>;

} // namespace bsgo
