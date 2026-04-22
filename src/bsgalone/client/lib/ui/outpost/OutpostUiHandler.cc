
#include "OutpostUiHandler.hh"
#include "IUiEventListener.hh"
#include "Palette.hh"
#include "UiTextMenu.hh"

namespace bsgalone::client {
namespace {
const std::unordered_set<UiEventType> RELEVANT_EVENT_TYPES = {
  UiEventType::LOADING_FINISHED,
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
    m_handler.onLoadingFinished();
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
  return m_tabsMenu->processUserInput(inputData);
}

void OutpostUiHandler::render(pge::Renderer &engine) const
{
  m_tabsMenu->render(engine);
}

void OutpostUiHandler::updateUi() {}

void OutpostUiHandler::registerToQueue(IUiEventQueueShPtr inputQueue)
{
  inputQueue->addListener(std::make_unique<UiEventListenerOutpostProxy>(*this));
}

namespace {
constexpr auto VIEW_LIST_WIDTH_TO_SCREEN_WIDTH_RATIO   = 0.2f;
constexpr auto VIEW_LIST_HEIGHT_TO_SCREEN_HEIGHT_RATIO = 0.5f;

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

  const pge::Vec2i pos{10, (dimensions.y - viewMenuHeight) / 2};
  const pge::Vec2i dims{viewMenuWidth, viewMenuHeight};

  ui::MenuConfig config{.pos = pos, .dims = dims};
  auto bg    = ui::bgConfigFromColor(pge::colors::BLANK);
  m_tabsMenu = std::make_unique<ui::UiMenu>(config, bg);
}

void OutpostUiHandler::initializeTabsMenuOptions()
{
  // TODO: It is not possible to get the player faction until the player
  // is logged in.
  // We should find a way to delay the initialization after the player
  // has logged in.
  // It can probably work similarly to the login UI handler
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

void OutpostUiHandler::onLoadingFinished() {}

} // namespace bsgalone::client
