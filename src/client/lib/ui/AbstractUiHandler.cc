
#include "AbstractUiHandler.hh"

namespace pge {

AbstractUiHandler::AbstractUiHandler(const std::string &name)
  : core::CoreObject(name)
{}

void AbstractUiHandler::connectToMessageQueue(bsgalone::core::IMessageQueue & /*messageQueue*/)
{
  // Intentionally empty.
}

} // namespace pge
