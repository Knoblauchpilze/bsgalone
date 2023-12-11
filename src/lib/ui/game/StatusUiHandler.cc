
#include "StatusUiHandler.hh"
#include "StringUtils.hh"

namespace pge {

StatusUiHandler::StatusUiHandler(const olc::vi2d &offset, const bsgo::Views &views)
  : IUiHandler("status")
  , m_offset(offset)
  , m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void StatusUiHandler::initializeMenus(const int /*width*/, const int /*height*/) {}

bool StatusUiHandler::processUserInput(UserInputData &inputData)
{
  for (const auto &menu : m_menus)
  {
    if (menu->processUserInput(inputData))
    {
      return true;
    }
  }

  return false;
}

void StatusUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_menus)
  {
    menu->render(engine.getRenderer());
  }
}

void StatusUiHandler::updateUi()
{
  if (!m_shipView->isReady())
  {
    return;
  }
}

} // namespace pge
