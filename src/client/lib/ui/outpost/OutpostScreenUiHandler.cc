
#include "OutpostScreenUiHandler.hh"
#include "Constants.hh"
#include "EquipMessage.hh"
#include "PurchaseMessage.hh"
#include "ScreenCommon.hh"
#include "UiTextMenu.hh"

namespace pge {

OutpostScreenUiHandler::OutpostScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("outpost")
  , bsgo::AbstractMessageListener({bsgo::MessageType::PURCHASE, bsgo::MessageType::EQUIP})
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
  const Vec2i pos{(width - UNDOCK_BUTTON_WIDTH) / 2, 10};
  const Vec2i dims{UNDOCK_BUTTON_WIDTH, 50};

  const MenuConfig config{.pos = pos, .dims = dims, .clickCallback = [this]() {
                            if (m_shipView->isReady())
                            {
                              m_shipView->undockPlayerShip();
                            }
                          }};

  auto bg         = bgConfigFromColor(colors::DARK_GREY);
  auto text       = textConfigFromColor("Undock", colors::WHITE);
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

void OutpostScreenUiHandler::render(Renderer &engine) const
{
  for (const auto &menu : m_menus)
  {
    menu->render(engine);
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

void OutpostScreenUiHandler::connectToMessageQueue(bsgo::IMessageQueue &messageQueue)
{
  m_lockerUi->connectToMessageQueue(messageQueue);
  m_shopUi->connectToMessageQueue(messageQueue);
  m_hangarUi->connectToMessageQueue(messageQueue);

  messageQueue.addListener(this);
}

void OutpostScreenUiHandler::onMessageReceived(const bsgo::IMessage &message)
{
  if (bsgo::MessageType::PURCHASE == message.type())
  {
    const auto &purchase = message.as<bsgo::PurchaseMessage>();
    if (bsgo::PurchaseState::COMPLETED == purchase.getPurchaseState())
    {
      m_refreshRequested = true;
    }
  }
  if (bsgo::MessageType::EQUIP == message.type())
  {
    const auto &equip = message.as<bsgo::EquipMessage>();
    if (bsgo::EquipState::COMPLETED == equip.getEquipState())
    {
      m_refreshRequested = true;
    }
  }
}

constexpr auto VIEW_LIST_WIDTH_TO_SCREEN_WIDTH_RATIO   = 0.2f;
constexpr auto VIEW_LIST_HEIGHT_TO_SCREEN_HEIGHT_RATIO = 0.5f;

void OutpostScreenUiHandler::generateGeneralMenu(const int width, const int height)
{
  const auto viewMenuWidth  = static_cast<int>(VIEW_LIST_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewMenuHeight = static_cast<int>(VIEW_LIST_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  const Vec2i pos{10, (height - viewMenuHeight) / 2};
  const Vec2i dims{viewMenuWidth, viewMenuHeight};

  MenuConfig config{.pos = pos, .dims = dims};
  auto bg             = bgConfigFromColor(colors::BLANK);
  m_menus[VIEWS_MENU] = std::make_unique<UiMenu>(config, bg);

  config.clickCallback = [this]() { setActiveScreen(ActiveScreen::SHOP); };
  bg                   = bgConfigFromColor(colors::DARK_COBALT_BLUE);
  auto text            = textConfigFromColor("Shop", colors::WHITE);
  auto menu            = std::make_unique<UiTextMenu>(config, bg, text);
  m_menus[VIEWS_MENU]->addMenu(std::move(menu));

  config.clickCallback = [this]() { setActiveScreen(ActiveScreen::LOCKER); };
  text                 = textConfigFromColor("Locker", colors::WHITE);
  menu                 = std::make_unique<UiTextMenu>(config, bg, text);
  m_menus[VIEWS_MENU]->addMenu(std::move(menu));

  config.clickCallback = [this]() { setActiveScreen(ActiveScreen::HANGAR); };
  text                 = textConfigFromColor("Hangar", colors::WHITE);
  menu                 = std::make_unique<UiTextMenu>(config, bg, text);
  m_menus[VIEWS_MENU]->addMenu(std::move(menu));
}

void OutpostScreenUiHandler::setActiveScreen(const ActiveScreen &screen)
{
  m_activeScreen = screen;
}

} // namespace pge
