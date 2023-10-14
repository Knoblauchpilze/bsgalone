
#include "IView.hh"

namespace bsgo {

IView::IView(const std::string &name)
  : utils::CoreObject(name)
{
  setService("view");
}

} // namespace bsgo
