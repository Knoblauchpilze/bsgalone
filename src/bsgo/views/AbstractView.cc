
#include "AbstractView.hh"

namespace bsgo {

AbstractView::AbstractView(const std::string &name)
  : utils::CoreObject(name)
  , IView()
{
  setService("view");
}

bool AbstractView::isReady() const noexcept
{
  return true;
}

} // namespace bsgo
