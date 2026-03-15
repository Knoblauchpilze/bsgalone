
#include "InputGameMessageAdapter.hh"

namespace bsgalone::client {

InputGameMessageAdapter::InputGameMessageAdapter()
  : AbstractMessageListener(core::allMessageTypesAsSet())
{}

void InputGameMessageAdapter::onEventReceived(const core::IMessage & /*event*/) {}

} // namespace bsgalone::client
