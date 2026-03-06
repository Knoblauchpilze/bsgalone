
#include "IViewListenerProxy.hh"

namespace bsgalone::client {

IViewListenerProxy::IViewListenerProxy(const OnUpdatedConsumer &listener)
  : IViewListener()
  , m_listener(std::move(listener))
{}

void IViewListenerProxy::onViewUpdated()
{
  if (m_listener)
  {
    m_listener();
  }
}

} // namespace bsgalone::client
