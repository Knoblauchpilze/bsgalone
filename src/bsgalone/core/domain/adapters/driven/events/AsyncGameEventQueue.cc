
#include "AsyncGameEventQueue.hh"
#include "AbstractAsyncEventQueue.hh"

namespace bsgalone::core {

auto createAsyncGameEventQueue(IGameEventQueuePtr queue) -> IGameEventQueueShPtr
{
  return std::make_shared<messaging::AbstractAsyncEventQueue<GameEventType, IGameEvent>>(
    std::move(queue));
}

} // namespace bsgalone::core
