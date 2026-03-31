
#include "AsyncUiEventQueue.hh"
#include "AbstractAsyncEventQueue.hh"

namespace bsgalone::client {

auto createAsyncUiEventQueue(IUiEventQueuePtr queue) -> IUiEventQueuePtr
{
  return std::make_unique<messaging::AbstractAsyncEventQueue<UiEventType, IUiEvent>>(
    std::move(queue));
}

} // namespace bsgalone::client
