
#include "OutpostScreenUiHandler.hh"
#include "Constants.hh"
#include "ScreenCommon.hh"

namespace pge {

OutpostScreenUiHandler::OutpostScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("outpost")
  , m_shipView(views.shipView)
  , m_lockerUi(std::make_unique<LockerUiHandler>(views))
  , m_shopUi(std::make_unique<ShopUiHandler>(views))
  , m_hangarUi(std::make_unique<HangarUiHandler>(views))
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void OutpostScreenUiHandler::initializeMenus(const int width, const int height)
{
  m_menus.resize(MenuItem::COUNT);

  constexpr auto UNDOCK_BUTTON_WIDTH = 100;
  const olc::vi2d pos{(width - UNDOCK_BUTTON_WIDTH) / 2, 10};
  const olc::vi2d dims{UNDOCK_BUTTON_WIDTH, 50};
  m_menus[UNDOCK]
    = generateMenu(pos, dims, "Undock", "undock", olc::VERY_DARK_GREY, {olc::WHITE}, true);
  m_menus[UNDOCK]->setSimpleAction([this](Game &g) {
    if (m_shipView->isReady())
    {
      m_shipView->undockPlayerShip();
      g.setScreen(Screen::GAME);
    }
  });

  generateGeneralMenu(width, height);

  m_lockerUi->initializeMenus(width, height);
  m_shopUi->initializeMenus(width, height);
  m_hangarUi->initializeMenus(width, height);
}

bool OutpostScreenUiHandler::processUserInput(UserInputData &inputData)
{
  bool out{false};
  switch (m_activeScreen)
  {
    case ActiveScreen::HANGAR:
      out = m_hangarUi->processUserInput(inputData);
      break;
    case ActiveScreen::SHOP:
      out = m_shopUi->processUserInput(inputData);
      break;
    case ActiveScreen::LOCKER:
      out = m_lockerUi->processUserInput(inputData);
      break;
  }
  if (!out)
  {
    for (const auto &menu : m_menus)
    {
      const auto ih = menu->processUserInput(inputData.controls, inputData.actions);
      out |= ih.relevant;
    }
  }

  return out;
}

void OutpostScreenUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_menus)
  {
    menu->render(engine.getRenderer());
  }

  switch (m_activeScreen)
  {
    case ActiveScreen::HANGAR:
      m_hangarUi->render(engine);
      break;
    case ActiveScreen::SHOP:
      m_shopUi->render(engine);
      break;
    case ActiveScreen::LOCKER:
      m_lockerUi->render(engine);
      break;
  }
}

void OutpostScreenUiHandler::updateUi()
{
  switch (m_activeScreen)
  {
    case ActiveScreen::HANGAR:
      m_hangarUi->updateUi();
      break;
    case ActiveScreen::SHOP:
      m_shopUi->updateUi();
      break;
    case ActiveScreen::LOCKER:
      m_lockerUi->updateUi();
      break;
  }
}

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
  menu->setAction([this]() { setActiveScreen(ActiveScreen::SHOP); });
  m_menus[VIEWS_MENU]->addMenu(menu);

  menu = generateMenu(pos, dims, "Locker", "locker", olc::DARK_COBALT_BLUE, {olc::WHITE}, true, true);
  menu->setAction([this]() { setActiveScreen(ActiveScreen::LOCKER); });
  m_menus[VIEWS_MENU]->addMenu(menu);

  menu = generateMenu(pos, dims, "Hangar", "hangar", olc::DARK_COBALT_BLUE, {olc::WHITE}, true, true);
  menu->setAction([this]() { setActiveScreen(ActiveScreen::HANGAR); });
  m_menus[VIEWS_MENU]->addMenu(menu);
}

void OutpostScreenUiHandler::setActiveScreen(const ActiveScreen &screen)
{
  m_activeScreen = screen;
}

} // namespace pge
