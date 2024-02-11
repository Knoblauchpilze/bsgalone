
#pragma once

#include "AbstractMessageConsumer.hh"
#include "EntityAddedMessage.hh"
#include "Services.hh"

namespace bsgo {

class EntityAddedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityAddedMessageConsumer(const Services &services);
  ~EntityAddedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
};

} // namespace bsgo
