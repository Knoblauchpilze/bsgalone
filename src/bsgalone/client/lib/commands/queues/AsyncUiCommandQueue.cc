
#include "AsyncUiCommandQueue.hh"
#include "AbstractAsyncEventQueue.hh"

namespace bsgalone::client {

auto createAsyncUiCommandQueue(IUiCommandQueuePtr queue) -> IUiCommandQueuePtr
{
  return std::make_unique<messaging::AbstractAsyncEventQueue<UiCommandType, IUiCommand>>(
    std::move(queue));
}

} // namespace bsgalone::client
