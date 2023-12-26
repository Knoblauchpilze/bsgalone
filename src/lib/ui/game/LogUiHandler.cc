
#include "LogUiHandler.hh"

namespace pge {

LogUiHandler::LogUiHandler(const bsgo::Views & /*views*/)
  : IUiHandler("log")
{}

void LogUiHandler::initializeMenus(const int /*width*/, const int /*height*/) {}

bool LogUiHandler::processUserInput(UserInputData & /*inputData*/)
{
  return false;
}

void LogUiHandler::render(SpriteRenderer & /*engine*/) const {}

void LogUiHandler::updateUi() {}

void LogUiHandler::reset() {}

} // namespace pge
