
#include "SynchronizedUiCommandQueue.hh"
#include "AbstractSynchronizedEventQueue.hh"

namespace bsgalone::client {

const auto UNIMPORTANT_COMMAND_TYPES = std::unordered_set<UiCommandType>{};

auto createSynchronizedUiCommandQueue() -> IUiCommandQueuePtr
{
  return std::make_unique<messaging::AbstractSynchronizedEventQueue<UiCommandType, IUiCommand>>(
    allUiCommandTypesAsSet(), UNIMPORTANT_COMMAND_TYPES);
}

} // namespace bsgalone::client
