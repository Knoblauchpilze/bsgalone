
#include "AsyncGameEventQueue.hh"
#include "AbstractAsyncEventQueue.hh"

namespace bsgalone::server {

auto createAsyncGameEventQueue(IGameEventQueuePtr queue) -> IGameEventQueueShPtr
{
  return std::make_shared<messaging::AbstractAsyncEventQueue<core::GameEventType, core::IGameEvent>>(
    std::move(queue));
}

} // namespace bsgalone::server
