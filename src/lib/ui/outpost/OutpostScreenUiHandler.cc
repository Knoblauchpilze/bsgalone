
#include "OutpostScreenUiHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

OutpostScreenUiHandler::OutpostScreenUiHandler()
  : IUiHandler("outpost")
{}

void OutpostScreenUiHandler::initializeMenus(const int width, const int height)
{
  m_menus.resize(MenuItem::COUNT);

  constexpr auto UNDOCK_BUTTON_WIDTH = 100;
  const olc::vi2d pos{(width - UNDOCK_BUTTON_WIDTH) / 2, 10};
  const olc::vi2d dims{UNDOCK_BUTTON_WIDTH, 50};
  m_menus[UNDOCK]
    = generateMenu(pos, dims, "Undock", "undock", olc::VERY_DARK_GREY, {olc::WHITE}, true);
  m_menus[UNDOCK]->setSimpleAction([this](Game &g) { g.setScreen(Screen::GAME); });

  generateGeneralMenu(width, height);
  generateShopView(width, height);
  generateLockerView(width, height);
  generateHangarView(width, height);
}

auto OutpostScreenUiHandler::processUserInput(const controls::State &c,
                                              std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  auto relevant{false};
  auto selected{false};

  for (const auto &menu : m_menus)
  {
    const auto ih = menu->processUserInput(c, actions);
    relevant      = (relevant || ih.relevant);
    selected      = (selected || ih.selected);
  }

  return menu::InputHandle{.relevant = relevant, .selected = selected};
}

void OutpostScreenUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_menus)
  {
    menu->render(engine.getRenderer());
  }
}

void OutpostScreenUiHandler::updateUi() {}

constexpr auto VIEW_LIST_WIDTH_TO_SCREEN_WIDTH_RATIO   = 0.2f;
constexpr auto VIEW_LIST_HEIGHT_TO_SCREEN_HEIGHT_RATIO = 0.5f;

void OutpostScreenUiHandler::generateGeneralMenu(const int width, const int height)
{
  const auto viewMenuWidth  = static_cast<int>(VIEW_LIST_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewMenuHeight = static_cast<int>(VIEW_LIST_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  const olc::vi2d pos{10, (height - viewMenuHeight) / 2};
  const olc::vi2d dims{viewMenuWidth, viewMenuHeight};
  m_menus[VIEWS_MENU]
    = generateMenu(pos, dims, "", "views", olc::VERY_DARK_COBALT_BLUE, {olc::WHITE});

  auto menu
    = generateMenu(pos, dims, "Shop", "shop", olc::DARK_COBALT_BLUE, {olc::WHITE}, true, true);
  m_menus[VIEWS_MENU]->addMenu(menu);

  menu = generateMenu(pos, dims, "Locker", "locker", olc::DARK_COBALT_BLUE, {olc::WHITE}, true, true);
  m_menus[VIEWS_MENU]->addMenu(menu);

  menu = generateMenu(pos, dims, "Hangar", "hangar", olc::DARK_COBALT_BLUE, {olc::WHITE}, true, true);
  m_menus[VIEWS_MENU]->addMenu(menu);
}

constexpr auto MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO   = 0.7f;
constexpr auto MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO = 0.8f;

constexpr auto VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS  = 20;
constexpr auto VIEW_TO_BOTTOM_OF_SCREEN_IN_PIXELS = 20;

void OutpostScreenUiHandler::generateShopView(const int width, const int height)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  const olc::vi2d pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                      height - viewHeight - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS};
  const olc::vi2d dims{viewWidth, viewHeight};
  m_menus[SHOP] = generateMenu(pos, dims, "", "shop", olc::VERY_DARK_RED, {olc::WHITE});
}

void OutpostScreenUiHandler::generateLockerView(const int width, const int height)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  const olc::vi2d pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                      height - viewHeight - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS};
  const olc::vi2d dims{viewWidth, viewHeight};
  m_menus[LOCKER] = generateMenu(pos, dims, "", "locker", olc::VERY_DARK_RED, {olc::WHITE});
}

void OutpostScreenUiHandler::generateHangarView(const int width, const int height)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  const olc::vi2d pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                      height - viewHeight - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS};
  const olc::vi2d dims{viewWidth, viewHeight};
  m_menus[HANGAR] = generateMenu(pos, dims, "", "hangar", olc::VERY_DARK_RED, {olc::WHITE});
}

} // namespace pge
