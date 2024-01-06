
#pragma once

#include "Coordinator.hh"
#include "IService.hh"
#include "Repositories.hh"

namespace bsgo {

class AbstractService : public IService
{
  public:
  AbstractService(const std::string &name,
                  const Repositories &repositories,
                  const CoordinatorShPtr &coordinator);
  ~AbstractService() override = default;

  bool isReady() const noexcept override;

  protected:
  Repositories m_repositories{};
  CoordinatorShPtr m_coordinator{};
};

} // namespace bsgo
