
#include "IInputHandler.hh"

namespace bsgalone::client {

IInputHandler::IInputHandler(const std::string &name)
  : ::core::CoreObject(name)
{
  setService("inputs");
}

} // namespace bsgalone::client
