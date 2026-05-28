
#include "ShopUiHandler.hh"
#include "IUiEventListener.hh"
#include "ScreenCommon.hh"

namespace bsgalone::client {
namespace {
const std::unordered_set<UiEventType> RELEVANT_EVENT_TYPES = {
  UiEventType::SHOP_READY,
};
}

class UiEventListenerShopProxy : public IUiEventListener
{
  public:
  explicit UiEventListenerShopProxy(ShopUiHandler &handler)
    : IUiEventListener()
    , m_handler(handler)
  {}

  ~UiEventListenerShopProxy() override = default;

  bool isEventRelevant(const UiEventType &type) const override
  {
    return RELEVANT_EVENT_TYPES.contains(type);
  }

  void onEventReceived(const IUiEvent & /*event*/) override
  {
    m_handler.onShopReady();
  }

  private:
  ShopUiHandler &m_handler;
};

ShopUiHandler::ShopUiHandler(IDataStoreShPtr dataStore, IUiEventQueueShPtr inputQueue)
  : IUiHandler()
  , m_dataStore(std::move(dataStore))
{
  if (nullptr == m_dataStore)
  {
    throw std::invalid_argument("Expected non null data store");
  }
  if (inputQueue == nullptr)
  {
    throw std::invalid_argument("Expected non null event queue");
  }

  registerToQueue(std::move(inputQueue));
}

namespace {
constexpr auto MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO   = 0.7f;
constexpr auto MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO = 0.8f;

constexpr auto VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS  = 20;
constexpr auto VIEW_TO_BOTTOM_OF_SCREEN_IN_PIXELS = 20;
} // namespace

void ShopUiHandler::initializeMenus(const pge::Vec2i &dimensions,
                                    pge::sprites::ITexturePack & /*texturesLoader*/)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * dimensions.x);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * dimensions.y);

  constexpr auto RESOURCES_MENU_HEIGHT = 30;
  pge::Vec2i pos{dimensions.x - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                 dimensions.y - viewHeight - VIEW_TO_BOTTOM_OF_SCREEN_IN_PIXELS};
  pge::Vec2i dims{viewWidth, RESOURCES_MENU_HEIGHT};

  m_resourcesMenu = generateBlankHorizontalMenu(pos, dims);

  pos.y += RESOURCES_MENU_HEIGHT;
  dims.y = viewHeight - RESOURCES_MENU_HEIGHT;
  m_menu = generateBlankVerticalMenu(pos, dims);
}

bool ShopUiHandler::processUserInput(ui::UserInputData &inputData)
{
  // The resources menu can't take input.
  return m_menu->processUserInput(inputData);
}

void ShopUiHandler::render(pge::Renderer &engine) const
{
  m_resourcesMenu->render(engine);
  m_menu->render(engine);
}

void ShopUiHandler::updateUi()
{
  // TODO: Add logic to update UI
}

void ShopUiHandler::registerToQueue(IUiEventQueueShPtr inputQueue)
{
  inputQueue->addListener(std::make_unique<UiEventListenerShopProxy>(*this));
}

void ShopUiHandler::onShopReady()
{
  // TODO: Should handle building the shop UI
}

} // namespace bsgalone::client
