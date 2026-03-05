
#include "AsyncMessageQueue.hh"
#include "AbstractAsyncEventQueue.hh"

namespace bsgo {

auto createAsyncMessageQueue(bsgalone::core::IMessageQueuePtr queue)
  -> bsgalone::core::IMessageQueueShPtr
{
  return std::make_shared<
    messaging::AbstractAsyncEventQueue<bsgalone::core::MessageType, bsgalone::core::IMessage>>(
    std::move(queue));
}

} // namespace bsgo
