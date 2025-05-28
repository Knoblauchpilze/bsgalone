
#include "AbstractView.hh"

namespace bsgo {

AbstractView::AbstractView(const std::string &name)
  : core::CoreObject(name)
  , IView()
{
  setService("view");
}

bool AbstractView::isReady() const noexcept
{
  return true;
}

} // namespace bsgo
