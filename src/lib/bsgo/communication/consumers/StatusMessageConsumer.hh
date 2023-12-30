
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Services.hh"

namespace bsgo {

class StatusMessageConsumer : public AbstractMessageConsumer
{
  public:
  StatusMessageConsumer(const Services &services);
  ~StatusMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  JumpServiceShPtr m_jumpService{};
};

} // namespace bsgo
