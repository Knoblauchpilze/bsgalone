
#include "ViewConsumerProxy.hh"

namespace bsgalone::client {

ViewConsumerProxy::ViewConsumerProxy(AbstractView &view)
  : AbstractMessageConsumer("proxy", view.relevantMessageTypes())
  , m_view(view)
{
  addModule(view.getName());
}

void ViewConsumerProxy::onEventReceived(const core::IMessage &message)
{
  m_view.onEventReceived(message);
}

} // namespace bsgalone::client
