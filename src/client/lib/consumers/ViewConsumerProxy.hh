
#pragma once

#include "AbstractMessageConsumer.hh"
#include "AbstractView.hh"

namespace pge {

class ViewConsumerProxy : public bsgo::AbstractMessageConsumer
{
  public:
  ViewConsumerProxy(AbstractView &view);
  ~ViewConsumerProxy() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  AbstractView &m_view;
};

} // namespace pge
