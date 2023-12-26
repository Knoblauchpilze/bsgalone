
#include "IUiHandler.hh"

namespace pge {

IUiHandler::IUiHandler(const std::string &name)
  : utils::CoreObject(name)
{}

void IUiHandler::reset()
{
  // Intentionally empty.
}

void IUiHandler::connectToMessageQueue(bsgo::IMessageQueue & /*messageQueue*/)
{
  // Intentionally empty.
}

} // namespace pge
