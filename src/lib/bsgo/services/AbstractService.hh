
#pragma once

#include "IService.hh"
#include "Repositories.hh"

namespace bsgo {

class AbstractService : public IService
{
  public:
  AbstractService(const std::string &name, const Repositories &repositories);
  ~AbstractService() override = default;

  private:
  Repositories m_repositories{};
};

} // namespace bsgo
