
#pragma once

#include "IService.hh"
#include "Repositories.hh"

namespace bsgalone::server {

class AbstractService : public IService
{
  public:
  AbstractService(const std::string &name, const core::Repositories &repositories);
  ~AbstractService() override = default;

  bool isReady() const noexcept override;

  // TODO: This method is meant as a temporary access to allow migrating to use
  // cases. The use cases will be instantiated by the message consumer and might
  // need the repositories.
  auto repositories() -> core::Repositories &;

  protected:
  core::Repositories m_repositories{};
};

} // namespace bsgalone::server
