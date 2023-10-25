
#include "IView.hh"

namespace bsgo {

IView::IView(const std::string &name, const CoordinatorShPtr &coordinator)
  : utils::CoreObject(name)
  , m_coordinator(coordinator)
{
  setService("view");
  if (nullptr == m_coordinator)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
}

} // namespace bsgo
