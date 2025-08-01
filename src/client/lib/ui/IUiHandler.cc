
#include "IUiHandler.hh"

namespace pge {

IUiHandler::IUiHandler(const std::string &name)
  : core::CoreObject(name)
{}

void IUiHandler::onLoadingTransition(const bsgo::LoadingTransition /*transition*/)
{
  // Intentionally empty.
}

void IUiHandler::connectToMessageQueue(bsgo::IMessageQueue & /*messageQueue*/)
{
  // Intentionally empty.
}

} // namespace pge
