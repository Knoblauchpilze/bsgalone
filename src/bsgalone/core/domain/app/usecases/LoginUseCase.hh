
#pragma once

#include "ForManagingSystem.hh"
#include "ForPublishingLoginData.hh"
#include "ForPublishingPlayerMessage.hh"

namespace bsgalone::core {

class LoginUseCase : public ForPublishingLoginData
{
  public:
  LoginUseCase(ForManagingSystemShPtr systemRepo, ForPublishingPlayerMessageShPtr playerEventBus);
  ~LoginUseCase() override = default;

  void publishLoginData(const Uuid playerDbId) override;

  private:
  ForManagingSystemShPtr m_systemRepo{};
  ForPublishingPlayerMessageShPtr m_playerEventBus{};
};

} // namespace bsgalone::core
