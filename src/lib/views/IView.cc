
#include "IView.hh"

namespace bsgo {

IView::IView(const std::string &name,
             const CoordinatorShPtr &coordinator,
             const Repositories &repositories)
  : utils::CoreObject(name)
  , m_coordinator(coordinator)
  , m_repositories(repositories)
{
  setService("view");
  if (nullptr == m_coordinator)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
}

} // namespace bsgo
