
#include "AbstractView.hh"

namespace bsgo {

AbstractView::AbstractView(const std::string &name,
                           const CoordinatorShPtr &coordinator,
                           const Repositories &repositories,
                           IMessageQueue *const messageQueue)
  : utils::CoreObject(name)
  , IView()
  , m_coordinator(coordinator)
  , m_repositories(repositories)
  , m_messageQueue(messageQueue)
{
  setService("view");
  if (nullptr == m_coordinator)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
  if (nullptr == messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

bool AbstractView::isReady() const noexcept
{
  return true;
}

} // namespace bsgo
