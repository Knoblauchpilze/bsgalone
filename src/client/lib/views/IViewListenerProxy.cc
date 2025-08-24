
#include "IViewListenerProxy.hh"

namespace pge {

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

} // namespace pge
