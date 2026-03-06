
#include "ShipStatusUiHandler.hh"
#include "IViewListenerProxy.hh"
#include "MessageListenerWrapper.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"
#include "TimeStepUtils.hh"
#include "TimeUtils.hh"

namespace bsgalone::client {

ShipStatusUiHandler::ShipStatusUiHandler(const Views &views)
  : AbstractUiHandler("ship_status")
  , AbstractMessageListener({core::MessageType::JUMP_REQUESTED, core::MessageType::JUMP_CANCELLED})
  , m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }

  subscribeToViews();
}

void ShipStatusUiHandler::initializeMenus(const int width,
                                          const int height,
                                          pge::sprites::TexturePack & /*texturesLoader*/)
{
  initializeThreatPanel(width, height);
  initializeJumpPanel(width, height);
}

bool ShipStatusUiHandler::processUserInput(ui::UserInputData &inputData)
{
  return m_jumpPanel->processUserInput(inputData);
}

void ShipStatusUiHandler::render(pge::Renderer &engine) const
{
  m_threatLabel->render(engine);
  m_jumpPanel->render(engine);
}

void ShipStatusUiHandler::updateUi()
{
  if (!m_shipView->isReady())
  {
    return;
  }

  updateThreatPanel();
  updateJumpPanel();
}

void ShipStatusUiHandler::connectToMessageQueue(core::IMessageQueue &messageQueue)
{
  auto listener = std::make_unique<MessageListenerWrapper>(this);
  messageQueue.addListener(std::move(listener));
}

void ShipStatusUiHandler::onEventReceived(const core::IMessage &message)
{
  if (!m_shipView->isReady())
  {
    return;
  }

  if (core::MessageType::JUMP_REQUESTED == message.type())
  {
    m_jumpStartTime = ::core::now();
  }
  if (core::MessageType::JUMP_CANCELLED == message.type())
  {
    m_jumpStartTime.reset();
  }
}

void ShipStatusUiHandler::subscribeToViews()
{
  auto consumer = [this]() { reset(); };

  auto listener = std::make_unique<IViewListenerProxy>(consumer);
  m_shipView->addListener(std::move(listener));
}

void ShipStatusUiHandler::reset()
{
  m_jumpStartTime.reset();
}

namespace {
const pge::Vec2i THREAT_UI_PIXEL_DIMENSION{100, 20};
const pge::Vec2i JUMP_UI_PIXEL_DIMENSION{100, 100};
constexpr auto REASONABLE_GAP_PIXELS = 15;
} // namespace

void ShipStatusUiHandler::initializeThreatPanel(const int width, const int height)
{
  const pge::Vec2i threatPixelPos{(width - THREAT_UI_PIXEL_DIMENSION.x) / 2,
                                  height - JUMP_UI_PIXEL_DIMENSION.y - REASONABLE_GAP_PIXELS
                                    - THREAT_UI_PIXEL_DIMENSION.y};

  const ui::MenuConfig config{.pos           = threatPixelPos,
                              .dims          = THREAT_UI_PIXEL_DIMENSION,
                              .highlightable = false};

  const auto bg   = ui::bgConfigFromColor(pge::colors::BLANK);
  const auto text = ui::textConfigFromColor("Threat", pge::colors::RED);

  auto label = std::make_unique<ui::UiTextMenu>(config, bg, text);
  ui::BlinkingMenuConfig blinkConfig{};
  blinkConfig.applyToBackground = false;
  m_threatLabel = std::make_unique<ui::UiBlinkingMenu>(blinkConfig, std::move(label));
}

void ShipStatusUiHandler::initializeJumpPanel(const int width, const int height)
{
  const pge::Vec2i jumpUiPixelPos{(width - JUMP_UI_PIXEL_DIMENSION.x) / 2,
                                  height - JUMP_UI_PIXEL_DIMENSION.y - REASONABLE_GAP_PIXELS};

  const ui::MenuConfig config{.pos           = jumpUiPixelPos,
                              .dims          = JUMP_UI_PIXEL_DIMENSION,
                              .highlightable = false};

  auto bg     = ui::bgConfigFromColor(semiOpaque(pge::colors::DARK_RED));
  m_jumpPanel = std::make_unique<ui::UiMenu>(config, bg);

  bg        = ui::bgConfigFromColor(pge::colors::BLANK);
  auto text = ui::textConfigFromColor("FTL Jump", pge::colors::WHITE);
  auto menu = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_jumpPanel->addMenu(std::move(menu));

  m_jumpPanel->addMenu(generateSpacer());

  text = ui::textConfigFromColor("Destination:", pge::colors::WHITE);
  menu = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_jumpPanel->addMenu(std::move(menu));

  text              = ui::textConfigFromColor("N/A", pge::colors::WHITE);
  menu              = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_jumpDestination = menu.get();
  m_jumpPanel->addMenu(std::move(menu));

  m_jumpPanel->addMenu(generateSpacer());

  text = ui::textConfigFromColor("Remaining:", pge::colors::WHITE);
  menu = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_jumpPanel->addMenu(std::move(menu));

  text       = ui::textConfigFromColor("N/A", pge::colors::WHITE);
  menu       = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_jumpTime = menu.get();
  m_jumpPanel->addMenu(std::move(menu));

  m_jumpPanel->addMenu(generateSpacer());

  m_jumpPanel->setVisible(false);
}

void ShipStatusUiHandler::updateThreatPanel()
{
  m_threatLabel->setVisible(m_shipView->isInThreat());
  m_threatLabel->update();
}

void ShipStatusUiHandler::updateJumpPanel()
{
  const auto jumping = m_shipView->isJumping();
  m_jumpPanel->setVisible(jumping);

  if (!jumping)
  {
    return;
  }
  if (!m_jumpStartTime)
  {
    error("Failed to process jump time with no registered jump");
  }

  const auto data     = m_shipView->getJumpData();
  const auto jumpTime = convertTickToDuration(data.jumpTime,
                                              m_shipView->gameSession().getTimeStep());

  m_jumpDestination->setText(data.systemName);

  const auto elapsedSinceJumpStarted = ::core::now() - *m_jumpStartTime;
  const auto remainingJumpTime       = jumpTime - elapsedSinceJumpStarted;

  m_jumpTime->setText(::core::durationToPrettyString(remainingJumpTime));
}

} // namespace bsgalone::client
