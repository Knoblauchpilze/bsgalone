
#pragma once

#include "Controls.hh"
#include "CoordinateFrame.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace pge {

class IInputHandler : public utils::CoreObject
{
  public:
  IInputHandler(const std::string &name);
  ~IInputHandler() override = default;

  virtual void processUserInput(const controls::State &controls, CoordinateFrame &frame) = 0;
  virtual void performAction(float x, float y, const controls::State &controls)          = 0;
};

using IInputHandlerPtr = std::unique_ptr<IInputHandler>;

} // namespace pge
