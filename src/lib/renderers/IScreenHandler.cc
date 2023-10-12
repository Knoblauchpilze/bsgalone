
#include "IScreenHandler.hh"

namespace pge {

IScreenHandler::IScreenHandler()
  : utils::CoreObject("handler")
{
  setService("bsgo");
}

} // namespace pge
