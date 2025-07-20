
#include "ViewConsumerProxy.hh"

namespace pge {

ViewConsumerProxy::ViewConsumerProxy(
  AbstractView &view,
  const std::unordered_set<bsgo::MessageType> &relevantMessageTypes)
  : bsgo::AbstractMessageConsumer("proxy", relevantMessageTypes)
  , m_view(view)
{
  addModule(view.getName());
}

void ViewConsumerProxy::onMessageReceived(const bsgo::IMessage &message)
{
  m_view.onMessageReceived(message);
}

} // namespace pge
