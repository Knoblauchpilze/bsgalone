
#pragma once

#include "ForManagingSystem.hh"
#include "ForPublishingLoginData.hh"
#include "ForPublishingPlayerMessage.hh"

namespace bsgalone::core {

class SendLoginDataUseCase : public ForPublishingLoginData
{
  public:
  SendLoginDataUseCase(ForManagingSystemShPtr systemRepo,
                       ForPublishingPlayerMessageShPtr playerEventBus);
  ~SendLoginDataUseCase() override = default;

  void publishLoginData(const Uuid playerDbId) override;

  private:
  ForManagingSystemShPtr m_systemRepo{};
  ForPublishingPlayerMessageShPtr m_playerEventBus{};
};

} // namespace bsgalone::core
