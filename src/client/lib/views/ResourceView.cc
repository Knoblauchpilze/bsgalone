
#include "ResourceView.hh"

namespace pge {

ResourceView::ResourceView(const bsgo::Repositories &repositories)
  : AbstractView("resource")
  , m_repositories(repositories)
{}

auto ResourceView::getResourceName(const bsgo::Uuid resource) const -> std::string
{
  return m_repositories.resourceRepository->findOneById(resource).name;
}

} // namespace pge
