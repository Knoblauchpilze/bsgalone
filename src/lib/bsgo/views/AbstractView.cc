
#include "AbstractView.hh"

namespace bsgo {

AbstractView::AbstractView(const std::string &name,
                           const CoordinatorShPtr &coordinator,
                           const Repositories &repositories)
  : utils::CoreObject(name)
  , IView()
  , m_coordinator(coordinator)
  , m_repositories(repositories)
{
  setService("view");
  if (nullptr == m_coordinator)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
}

bool AbstractView::isReady() const noexcept
{
  return true;
}

} // namespace bsgo
