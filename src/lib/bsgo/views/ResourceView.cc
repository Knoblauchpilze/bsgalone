
#include "ResourceView.hh"

namespace bsgo {

ResourceView::ResourceView(const CoordinatorShPtr &coordinator, const Repositories &repositories)
  : AbstractView("resource", coordinator, repositories)
{}

auto ResourceView::getResourceName(const Uuid &resource) const -> std::string
{
  return m_repositories.resourceRepository->findOneById(resource).name;
}

} // namespace bsgo
