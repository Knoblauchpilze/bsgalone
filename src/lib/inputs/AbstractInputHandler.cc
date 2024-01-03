
#include "AbstractInputHandler.hh"

namespace pge {

AbstractInputHandler::AbstractInputHandler(const std::string &name,
                                           bsgo::IMessageQueue *const messageQueue)
  : IInputHandler(name)
  , m_messageQueue(messageQueue)
{
  if (nullptr == messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

} // namespace pge
