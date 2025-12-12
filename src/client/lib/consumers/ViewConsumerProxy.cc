
#include "ViewConsumerProxy.hh"

namespace pge {

ViewConsumerProxy::ViewConsumerProxy(AbstractView &view)
  : bsgo::AbstractMessageConsumer("proxy", view.relevantMessageTypes())
  , m_view(view)
{
  addModule(view.getName());
}

void ViewConsumerProxy::onMessageReceived(const bsgo::IMessage &message)
{
  m_view.onMessageReceived(message);
}

} // namespace pge
