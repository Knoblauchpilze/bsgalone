
#pragma once

#include "AbstractMessageConsumer.hh"
#include "AbstractView.hh"

namespace pge {

class ViewConsumerProxy : public bsgalone::core::AbstractMessageConsumer
{
  public:
  ViewConsumerProxy(AbstractView &view);
  ~ViewConsumerProxy() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  AbstractView &m_view;
};

} // namespace pge
