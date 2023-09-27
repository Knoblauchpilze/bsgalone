
#include "SystemView.hh"

namespace bsgo {

SystemView::SystemView()
  : utils::CoreObject("system")
{
  setService("view");
}

} // namespace bsgo
