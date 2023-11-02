
#include "LockerUiHandler.hh"
#include "Constants.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"

namespace pge {

LockerUiHandler::LockerUiHandler(const bsgo::Views &views)
  : IUiHandler("locker")
  , m_playerView(views.playerView)
{
  if (nullptr == m_playerView)
  {
    throw std::invalid_argument("Expected non null player view");
  }
}

void LockerUiHandler::initializeMenus(const int width, const int height)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  const olc::vi2d pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                      height - viewHeight - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS};
  const olc::vi2d dims{viewWidth, viewHeight};
  m_menu = generateMenu(pos, dims, "", "locker", olc::DARK_YELLOW, {olc::WHITE});

  generateResourcesMenus();
}

auto LockerUiHandler::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void LockerUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
}

void LockerUiHandler::updateUi() {}

void LockerUiHandler::generateResourcesMenus()
{
  const auto resources = m_playerView->getPlayerResources();
  for (const auto &resource : resources)
  {
    auto text = resource.name + ": " + floatToStr(std::floor(resource.amount), 0);
    auto menu = generateMenu(olc::vi2d{}, olc::vi2d{}, text, resource.name, olc::VERY_DARK_GREEN);
    m_menu->addMenu(menu);
    m_resources.push_back(menu);
  }
}

} // namespace pge
