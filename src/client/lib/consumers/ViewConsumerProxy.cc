
#include "ViewConsumerProxy.hh"

namespace pge {

ViewConsumerProxy::ViewConsumerProxy(AbstractView &view)
  : AbstractMessageConsumer("proxy", view.relevantMessageTypes())
  , m_view(view)
{
  addModule(view.getName());
}

void ViewConsumerProxy::onEventReceived(const bsgalone::core::IMessage &message)
{
  m_view.onEventReceived(message);
}

} // namespace pge
