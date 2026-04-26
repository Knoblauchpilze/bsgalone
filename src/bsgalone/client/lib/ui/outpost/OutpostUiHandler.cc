
#include "OutpostUiHandler.hh"
#include "IUiEventListener.hh"
#include "LogoutCommand.hh"
#include "Palette.hh"
#include "UiTextMenu.hh"
#include "UndockCommand.hh"

namespace bsgalone::client {
namespace {
const std::unordered_set<UiEventType> RELEVANT_EVENT_TYPES = {
  UiEventType::LOGIN_SUCCEEDED,
};
}

class UiEventListenerOutpostProxy : public IUiEventListener
{
  public:
  explicit UiEventListenerOutpostProxy(OutpostUiHandler &handler)
    : IUiEventListener()
    , m_handler(handler)
  {}

  ~UiEventListenerOutpostProxy() override = default;

  bool isEventRelevant(const UiEventType &type) const override
  {
    return RELEVANT_EVENT_TYPES.contains(type);
  }

  void onEventReceived(const IUiEvent & /*event*/) override
  {
    m_handler.onLoginSucceeded();
  }

  private:
  OutpostUiHandler &m_handler;
};

OutpostUiHandler::OutpostUiHandler(IDataStoreShPtr dataStore,
                                   IUiEventQueueShPtr inputQueue,
                                   IUiCommandQueueShPtr outputQueue)
  : IUiHandler()
  , m_dataStore(std::move(dataStore))
  , m_queue(std::move(outputQueue))
{
  if (nullptr == m_dataStore)
  {
    throw std::invalid_argument("Expected non null data store");
  }
  if (inputQueue == nullptr)
  {
    throw std::invalid_argument("Expected non null event queue");
  }
  if (nullptr == m_queue)
  {
    throw std::invalid_argument("Expected non null command queue");
  }

  registerToQueue(std::move(inputQueue));
}

void OutpostUiHandler::initializeMenus(const pge::Vec2i &dimensions,
                                       pge::sprites::TexturePack & /*texturesLoader*/)
{
  generateTabsMenu(dimensions);
}

bool OutpostUiHandler::processUserInput(ui::UserInputData &inputData)
{
  if (m_tabsMenu->processUserInput(inputData))
  {
    return true;
  }

  if (m_undockButton->processUserInput(inputData))
  {
    return true;
  }

  return m_quitButton->processUserInput(inputData);
}

void OutpostUiHandler::render(pge::Renderer &engine) const
{
  m_tabsMenu->render(engine);
  m_undockButton->render(engine);
  m_quitButton->render(engine);
}

void OutpostUiHandler::updateUi()
{
  // Intentionally empty: nothing to update at this step.
}

void OutpostUiHandler::registerToQueue(IUiEventQueueShPtr inputQueue)
{
  inputQueue->addListener(std::make_unique<UiEventListenerOutpostProxy>(*this));
}

namespace {
constexpr auto VIEW_LIST_WIDTH_TO_SCREEN_WIDTH_RATIO   = 0.2f;
constexpr auto VIEW_LIST_HEIGHT_TO_SCREEN_HEIGHT_RATIO = 0.5f;

constexpr auto UNDOCK_BUTTON_WIDTH  = 100;
constexpr auto UNDOCK_BUTTON_HEIGHT = 50;

constexpr auto REASONABLE_PIXELS_GAP = 40;
constexpr auto LOGOUT_BUTTON_WIDTH   = 80;
constexpr auto LOGOUT_BUTTON_HEIGHT  = 30;

void addTabSectionToMenu(ui::UiMenu &mainPanel,
                         const std::string &tabName,
                         const Palette &palette,
                         ui::ClickCallback callback)
{
  ui::MenuConfig config{};

  config.clickCallback = callback;
  const auto bg        = ui::bgConfigFromColor(palette.almostOpaqueColor);
  auto text            = ui::textConfigFromColor(tabName, pge::colors::WHITE);
  auto menu            = std::make_unique<ui::UiTextMenu>(config, bg, text);

  mainPanel.addMenu(std::move(menu));
}
} // namespace

void OutpostUiHandler::generateTabsMenu(const pge::Vec2i &dimensions)
{
  const auto viewMenuWidth = static_cast<int>(VIEW_LIST_WIDTH_TO_SCREEN_WIDTH_RATIO * dimensions.x);
  const auto viewMenuHeight = static_cast<int>(VIEW_LIST_HEIGHT_TO_SCREEN_HEIGHT_RATIO
                                               * dimensions.y);

  pge::Vec2i pos{10, (dimensions.y - viewMenuHeight) / 2};
  const pge::Vec2i dims{viewMenuWidth, viewMenuHeight};

  ui::MenuConfig config{.pos = pos, .dims = dims};
  auto bg    = ui::bgConfigFromColor(pge::colors::BLANK);
  m_tabsMenu = std::make_unique<ui::UiMenu>(config, bg);

  config = ui::MenuConfig{
    .pos           = pge::Vec2i{(dimensions.x - UNDOCK_BUTTON_WIDTH) / 2, 10},
    .dims          = pge::Vec2i{UNDOCK_BUTTON_WIDTH, UNDOCK_BUTTON_HEIGHT},
    .clickCallback = [this]() { onUndockRequested(); },
  };

  bg             = ui::bgConfigFromColor(pge::colors::DARK_GREY);
  auto text      = ui::textConfigFromColor("Undock", pge::colors::WHITE);
  m_undockButton = std::make_unique<ui::UiTextMenu>(config, bg, text);

  pos    = pge::Vec2i{config.pos.x + config.dims.x + REASONABLE_PIXELS_GAP,
                   config.pos.y + ((UNDOCK_BUTTON_HEIGHT - LOGOUT_BUTTON_HEIGHT) / 2)};
  config = ui::MenuConfig{
    .pos           = pos,
    .dims          = pge::Vec2i{LOGOUT_BUTTON_WIDTH, LOGOUT_BUTTON_HEIGHT},
    .clickCallback = [this]() { onLogoutRequested(); },
  };

  bg           = ui::bgConfigFromColor(pge::colors::DARK_RED);
  text         = ui::textConfigFromColor("Logout", pge::colors::VERY_DARK_RED);
  m_quitButton = std::make_unique<ui::UiTextMenu>(config, bg, text);
}

void OutpostUiHandler::initializeTabsMenuOptions()
{
  m_tabsMenu->clearChildren();

  const auto palette = generatePaletteForFaction(m_dataStore->getPlayerFaction());

  ui::ClickCallback callback = [this]() { setActiveScreen(ActiveScreen::SHOP); };
  addTabSectionToMenu(*m_tabsMenu, "Shop", palette, callback);

  callback = [this]() { setActiveScreen(ActiveScreen::LOCKER); };
  addTabSectionToMenu(*m_tabsMenu, "Locker", palette, callback);

  callback = [this]() { setActiveScreen(ActiveScreen::HANGAR); };
  addTabSectionToMenu(*m_tabsMenu, "Hangar", palette, callback);
}

void OutpostUiHandler::setActiveScreen(const ActiveScreen screen)
{
  m_activeScreen = screen;
}

void OutpostUiHandler::onUndockRequested()
{
  m_queue->pushEvent(std::make_unique<UndockCommand>());
}

void OutpostUiHandler::onLogoutRequested()
{
  m_queue->pushEvent(std::make_unique<LogoutCommand>());
}

void OutpostUiHandler::onLoginSucceeded()
{
  initializeTabsMenuOptions();
}

} // namespace bsgalone::client
