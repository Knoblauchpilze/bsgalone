
#include "AbstractService.hh"

namespace bsgalone::server {

AbstractService::AbstractService(const std::string &name, const core::Repositories &repositories)
  : IService(name)
  , m_repositories(repositories)
{}

bool AbstractService::isReady() const noexcept
{
  return true;
}

} // namespace bsgalone::server
