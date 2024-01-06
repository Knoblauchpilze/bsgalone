
#include "IInputHandler.hh"

namespace pge {

IInputHandler::IInputHandler(const std::string &name)
  : utils::CoreObject(name)
{
  setService("inputs");
}

} // namespace pge
