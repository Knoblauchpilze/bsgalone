
#include "ResourceView.hh"

namespace bsgo {

ResourceView::ResourceView(const CoordinatorShPtr &coordinator,
                           const Repositories &repositories,
                           const IMessageQueue *messageQueue)
  : AbstractView("resource", coordinator, repositories, messageQueue)
{}

auto ResourceView::getResourceName(const Uuid &resource) const -> std::string
{
  return m_repositories.resourceRepository->findOneById(resource).name;
}

} // namespace bsgo
