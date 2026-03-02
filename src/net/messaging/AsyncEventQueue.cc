
#include "AsyncEventQueue.hh"
#include "AbstractAsyncEventQueue.hh"

namespace net {

auto createAsyncEventQueue(INetworkEventQueuePtr queue) -> INetworkEventQueueShPtr
{
  return std::make_shared<messaging::AbstractAsyncEventQueue<EventType, IEvent>>(std::move(queue));
}

} // namespace net
