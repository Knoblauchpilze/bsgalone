
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class LockerService : public AbstractService
{
  public:
  LockerService(const Repositories &repositories);
  ~LockerService() override = default;

  void setPlayerDbId(const Uuid &player);
  bool isReady() const noexcept override;

  private:
  std::optional<Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;
};

using LockerServiceShPtr = std::shared_ptr<LockerService>;

} // namespace bsgo
