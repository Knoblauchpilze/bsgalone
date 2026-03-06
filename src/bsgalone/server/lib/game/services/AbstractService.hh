
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

  protected:
  core::Repositories m_repositories{};
};

} // namespace bsgalone::server
