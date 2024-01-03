
#include "AbstractInputHandler.hh"

namespace pge {

AbstractInputHandler::AbstractInputHandler(const std::string &name,
                                           bsgo::IMessageQueue *const messageQueue)
  : IInputHandler(name)
  , m_messageQueue(messageQueue)
{}

} // namespace pge
