
#pragma once

#include "AbstractMessageConsumer.hh"
#include "AbstractView.hh"

namespace bsgalone::client {

class ViewConsumerProxy : public core::AbstractMessageConsumer
{
  public:
  ViewConsumerProxy(AbstractView &view);
  ~ViewConsumerProxy() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  AbstractView &m_view;
};

} // namespace bsgalone::client
