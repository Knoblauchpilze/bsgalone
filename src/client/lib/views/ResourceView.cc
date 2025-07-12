
#include "ResourceView.hh"

namespace bsgo {

ResourceView::ResourceView(const Repositories &repositories)
  : AbstractView("resource")
  , m_repositories(repositories)
{}

auto ResourceView::getResourceName(const Uuid resource) const -> std::string
{
  return m_repositories.resourceRepository->findOneById(resource).name;
}

} // namespace bsgo
