
#include "LogUiHandler.hh"

namespace pge {

LogUiHandler::LogUiHandler(const bsgo::Views & /*views*/)
  : IUiHandler("log")
  , AbstractMessageListener({bsgo::MessageType::SYSTEM})
{}

void LogUiHandler::initializeMenus(const int /*width*/, const int /*height*/) {}

bool LogUiHandler::processUserInput(UserInputData & /*inputData*/)
{
  return false;
}

void LogUiHandler::render(SpriteRenderer & /*engine*/) const {}

void LogUiHandler::updateUi() {}

void LogUiHandler::reset() {}

void LogUiHandler::connectToMessageQueue(bsgo::IMessageQueue &messageQueue)
{
  messageQueue.addListener(this);
}

void LogUiHandler::onMessageReceived(const bsgo::IMessage &message)
{
  warn("should handle message with type " + bsgo::str(message.type()));
}

} // namespace pge
