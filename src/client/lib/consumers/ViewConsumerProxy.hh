
#pragma once

#include "AbstractMessageConsumer.hh"
#include "AbstractView.hh"
#include <unordered_set>

namespace pge {

class ViewConsumerProxy : public bsgo::AbstractMessageConsumer
{
  public:
  ViewConsumerProxy(AbstractView &view,
                    const std::unordered_set<bsgo::MessageType> &relevantMessageTypes);
  ~ViewConsumerProxy() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  AbstractView &m_view;
};

} // namespace pge
