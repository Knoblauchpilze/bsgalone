
#pragma once

#include "Controls.hh"
#include "CoordinateFrame.hh"
#include "InputHandle.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace pge {

class IInputHandler : public utils::CoreObject
{
  public:
  IInputHandler(const std::string &name);
  virtual ~IInputHandler() = default;

  virtual void processUserInput(const controls::State &c, CoordinateFrame &frame) = 0;
  virtual void performAction(float x, float y, const controls::State &state)      = 0;
};

using IInputHandlerPtr = std::unique_ptr<IInputHandler>;

} // namespace pge
