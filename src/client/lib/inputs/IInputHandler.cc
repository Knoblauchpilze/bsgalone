
#include "IInputHandler.hh"

namespace pge {

IInputHandler::IInputHandler(const std::string &name)
  : core::CoreObject(name)
{
  setService("inputs");
}

} // namespace pge
