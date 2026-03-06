
#include "AsyncMessageQueue.hh"
#include "AbstractAsyncEventQueue.hh"

namespace bsgalone::core {

auto createAsyncMessageQueue(IMessageQueuePtr queue) -> IMessageQueueShPtr
{
  return std::make_shared<messaging::AbstractAsyncEventQueue<MessageType, IMessage>>(
    std::move(queue));
}

} // namespace bsgalone::core
