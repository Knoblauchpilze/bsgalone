
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Services.hh"

namespace bsgo {

class ShipDeletedMessageConsumer : public AbstractMessageConsumer
{
  public:
  ShipDeletedMessageConsumer(const Services &services);
  ~ShipDeletedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
};

} // namespace bsgo
