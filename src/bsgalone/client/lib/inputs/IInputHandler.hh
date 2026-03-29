
#pragma once

#include "Controls.hh"
#include "CoordinateFrame.hh"
#include "CoreObject.hh"
#include <memory>

namespace bsgalone::client {

class IInputHandler
{
  public:
  IInputHandler()          = default;
  virtual ~IInputHandler() = default;

  virtual void processUserInput(const pge::controls::State &controls, pge::CoordinateFrame &frame)
    = 0;
  virtual void performAction(float x, float y, const pge::controls::State &controls) = 0;
};

using IInputHandlerPtr = std::unique_ptr<IInputHandler>;

} // namespace bsgalone::client
