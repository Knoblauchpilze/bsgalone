
#include "AbstractService.hh"

namespace bsgo {

AbstractService::AbstractService(const std::string &name, const Repositories &repositories)
  : IService(name)
  , m_repositories(repositories)
{}

bool AbstractService::isReady() const noexcept
{
  return true;
}

} // namespace bsgo
