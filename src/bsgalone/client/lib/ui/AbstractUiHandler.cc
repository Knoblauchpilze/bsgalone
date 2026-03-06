
#include "AbstractUiHandler.hh"

namespace bsgalone::client {

AbstractUiHandler::AbstractUiHandler(const std::string &name)
  : ::core::CoreObject(name)
{}

void AbstractUiHandler::connectToMessageQueue(core::IMessageQueue & /*messageQueue*/)
{
  // Intentionally empty.
}

} // namespace bsgalone::client
