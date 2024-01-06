
#include "AbstractService.hh"

namespace bsgo {

AbstractService::AbstractService(const std::string &name,
                                 const Repositories &repositories,
                                 const CoordinatorShPtr &coordinator)
  : IService(name)
  , m_repositories(repositories)
  , m_coordinator(coordinator)
{
  if (nullptr == m_coordinator)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
}

bool AbstractService::isReady() const noexcept
{
  return true;
}

} // namespace bsgo
