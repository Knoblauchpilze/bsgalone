
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"

namespace pge {

class AbstractGameMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  AbstractGameMessageConsumer(const bool active, bsgo::AbstractMessageConsumerPtr consumer);
  ~AbstractGameMessageConsumer() override = default;

  void activate(const bool active = true);

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bool m_active{true};
  bsgo::AbstractMessageConsumerPtr m_consumer{};
};

} // namespace pge
