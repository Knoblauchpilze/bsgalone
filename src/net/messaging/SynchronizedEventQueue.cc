
#include "SynchronizedEventQueue.hh"
#include "AbstractSynchronizedEventQueue.hh"

namespace net {

const auto UNIMPORTANT_EVENT_TYPES = std::unordered_set<NetworkEventType>{
  NetworkEventType::DATA_RECEIVED,
  NetworkEventType::DATA_SENT,
};

auto createSynchronizedEventQueue() -> INetworkEventQueuePtr
{
  return std::make_unique<messaging::AbstractSynchronizedEventQueue<NetworkEventType, INetworkEvent>>(
    allNetworkEventTypesAsSet(), UNIMPORTANT_EVENT_TYPES);
}

} // namespace net
