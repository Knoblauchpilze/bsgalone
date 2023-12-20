
#include "OutpostScreenUiHandler.hh"
#include "Constants.hh"
#include "ScreenCommon.hh"
#include "UiTextMenu.hh"

namespace pge {

OutpostScreenUiHandler::OutpostScreenUiHandler(const bsgo::Views &views,
                                               const bsgo::Services &services)
  : IUiHandler("outpost")
  , m_shipView(views.shipView)
  , m_lockerUi(std::make_unique<LockerUiHandler>(views, services))
  , m_shopUi(std::make_unique<ShopUiHandler>(views, services))
  , m_hangarUi(std::make_unique<HangarUiHandler>(views))
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }

  m_shopUi->onItemPurchased
    .connect_member<OutpostScreenUiHandler>(this, &OutpostScreenUiHandler::onChildUiChanged);
  m_lockerUi->onItemEquiped
    .connect_member<OutpostScreenUiHandler>(this, &OutpostScreenUiHandler::onChildUiChanged);
  m_lockerUi->onItemUnequiped
    .connect_member<OutpostScreenUiHandler>(this, &OutpostScreenUiHandler::onChildUiChanged);
}

void OutpostScreenUiHandler::initializeMenus(const int width, const int height)
{
  m_menus.resize(MenuItem::COUNT);

  constexpr auto UNDOCK_BUTTON_WIDTH = 100;
  const olc::vi2d pos{(width - UNDOCK_BUTTON_WIDTH) / 2, 10};
  const olc::vi2d dims{UNDOCK_BUTTON_WIDTH, 50};

  const MenuConfig config{.pos = pos, .dims = dims, .gameClickCallback = [this](Game &g) {
                            if (m_shipView->isReady())
                            {
                              m_shipView->undockPlayerShip();
                              g.setScreen(Screen::GAME);
                            }
                          }};

  auto bg         = bgConfigFromColor(olc::DARK_GREY);
  auto text       = textConfigFromColor("Undock", olc::WHITE);
  m_menus[UNDOCK] = std::make_unique<UiTextMenu>(config, bg, text);

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
      if (menu->processUserInput(inputData))
      {
        return true;
      }
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
  if (m_refreshRequested)
  {
    reset();
    m_refreshRequested = false;
  }

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

void OutpostScreenUiHandler::reset()
{
  m_lockerUi->reset();
  m_shopUi->reset();
  m_hangarUi->reset();
}

constexpr auto VIEW_LIST_WIDTH_TO_SCREEN_WIDTH_RATIO   = 0.2f;
constexpr auto VIEW_LIST_HEIGHT_TO_SCREEN_HEIGHT_RATIO = 0.5f;

void OutpostScreenUiHandler::generateGeneralMenu(const int width, const int height)
{
  const auto viewMenuWidth  = static_cast<int>(VIEW_LIST_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewMenuHeight = static_cast<int>(VIEW_LIST_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  const olc::vi2d pos{10, (height - viewMenuHeight) / 2};
  const olc::vi2d dims{viewMenuWidth, viewMenuHeight};

  MenuConfig config{.pos = pos, .dims = dims};
  auto bg             = bgConfigFromColor(olc::BLANK);
  m_menus[VIEWS_MENU] = std::make_unique<UiMenu>(config, bg);

  config.clickCallback = [this]() { setActiveScreen(ActiveScreen::SHOP); };
  bg                   = bgConfigFromColor(olc::DARK_COBALT_BLUE);
  auto text            = textConfigFromColor("Shop", olc::WHITE);
  auto menu            = std::make_unique<UiTextMenu>(config, bg, text);
  m_menus[VIEWS_MENU]->addMenu(std::move(menu));

  config.clickCallback = [this]() { setActiveScreen(ActiveScreen::LOCKER); };
  text                 = textConfigFromColor("Locker", olc::WHITE);
  menu                 = std::make_unique<UiTextMenu>(config, bg, text);
  m_menus[VIEWS_MENU]->addMenu(std::move(menu));

  config.clickCallback = [this]() { setActiveScreen(ActiveScreen::HANGAR); };
  text                 = textConfigFromColor("Hangar", olc::WHITE);
  menu                 = std::make_unique<UiTextMenu>(config, bg, text);
  m_menus[VIEWS_MENU]->addMenu(std::move(menu));
}

void OutpostScreenUiHandler::setActiveScreen(const ActiveScreen &screen)
{
  m_activeScreen = screen;
}

void OutpostScreenUiHandler::onChildUiChanged()
{
  m_refreshRequested = true;
}

} // namespace pge
