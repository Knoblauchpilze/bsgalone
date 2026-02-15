
#include "IUiHandler.hh"

namespace pge {

IUiHandler::IUiHandler(const std::string &name)
  : core::CoreObject(name)
{}

void IUiHandler::connectToMessageQueue(bsgalone::core::IMessageQueue & /*messageQueue*/)
{
  // Intentionally empty.
}

} // namespace pge
