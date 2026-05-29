
#include "HangarUiHandler.hh"
#include "IUiEventListener.hh"
#include "ScreenCommon.hh"

#include <iostream>

namespace bsgalone::client {

class UiEventListenerHangarProxy : public IUiEventListener
{
  public:
  explicit UiEventListenerHangarProxy(HangarUiHandler &handler)
    : IUiEventListener()
    , m_handler(handler)
  {}

  ~UiEventListenerHangarProxy() override = default;

  bool isEventRelevant(const UiEventType &type) const override
  {
    return type == UiEventType::HANGAR_READY;
  }

  void onEventReceived(const IUiEvent & /*event*/) override
  {
    m_handler.onHangarReady();
  }

  private:
  HangarUiHandler &m_handler;
};

HangarUiHandler::HangarUiHandler(IDataStoreShPtr dataStore, IUiEventQueueShPtr inputQueue)
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

void HangarUiHandler::initializeMenus(const pge::Vec2i &dimensions,
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

bool HangarUiHandler::processUserInput(ui::UserInputData &inputData)
{
  // The resources menu can't take input.
  return m_menu->processUserInput(inputData);
}

void HangarUiHandler::render(pge::Renderer &engine) const
{
  std::cout << "render hangar\n";
  m_resourcesMenu->render(engine);
  m_menu->render(engine);
}

void HangarUiHandler::updateUi()
{
  // TODO: Add logic to update UI
}

void HangarUiHandler::registerToQueue(IUiEventQueueShPtr inputQueue)
{
  inputQueue->addListener(std::make_unique<UiEventListenerHangarProxy>(*this));
}

void HangarUiHandler::onHangarReady()
{
  // TODO: Should handle building the shop UI
}

} // namespace bsgalone::client
