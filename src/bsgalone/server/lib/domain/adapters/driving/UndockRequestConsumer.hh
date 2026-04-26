
#pragma once

#include "CoreObject.hh"
#include "ForExecutingUndock.hh"
#include "IMessageListener.hh"

namespace bsgalone::server {

class UndockRequestConsumer : public core::IMessageListener, public ::core::CoreObject
{
  public:
  explicit UndockRequestConsumer(ForExecutingUndockPtr undockUseCase);
  ~UndockRequestConsumer() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &message) override;

  private:
  ForExecutingUndockPtr m_useCase{};
};

} // namespace bsgalone::server
