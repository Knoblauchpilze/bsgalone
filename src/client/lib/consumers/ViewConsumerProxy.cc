
#include "ViewConsumerProxy.hh"

namespace pge {

ViewConsumerProxy::ViewConsumerProxy(AbstractView &view)
  : bsgo::AbstractMessageConsumer("proxy", view.relevantMessageTypes())
  , m_view(view)
{
  addModule(view.getName());
}

void ViewConsumerProxy::onMessageReceived(const bsgalone::core::IMessage &message)
{
  m_view.onMessageReceived(message);
}

} // namespace pge
