
#include "StatusUiHandler.hh"
#include "LogoutCommand.hh"
#include "ScreenCommon.hh"

namespace bsgalone::client {

StatusUiHandler::StatusUiHandler(IUiCommandQueueShPtr outputQueue)
  : IUiHandler()
  , m_queue(std::move(outputQueue))
{}

namespace {
const pge::Vec2i STATUS_UI_PIXEL_POS{5, 5};
constexpr auto STATUS_MENU_HEIGHT = 20;
} // namespace

void StatusUiHandler::initializeMenus(const pge::Vec2i &dimensions,
                                      pge::sprites::TexturePack & /*texturesLoader*/)
{
  const pge::Vec2i statusMenuDims{dimensions.x - 2 * STATUS_UI_PIXEL_POS.x, STATUS_MENU_HEIGHT};

  const ui::MenuConfig config{.pos           = STATUS_UI_PIXEL_POS,
                              .dims          = statusMenuDims,
                              .layout        = ui::MenuLayout::HORIZONTAL,
                              .highlightable = false};
  auto bg     = ui::bgConfigFromColor(almostTransparent(pge::colors::WHITE));
  m_statusBar = std::make_unique<ui::UiMenu>(config, bg);

  m_statusBar->addMenu(generateSpacer());
  m_statusBar->addMenu(generateSpacer());
  m_statusBar->addMenu(generateSpacer());

  generateLogoutButton(dimensions);
  generateLogoutConfirmationPanel(dimensions);
}

bool StatusUiHandler::processUserInput(ui::UserInputData &inputData)
{
  auto out = m_statusBar->processUserInput(inputData);
  out |= m_logoutConfirmation->processUserInput(inputData);
  return out;
}

void StatusUiHandler::render(pge::Renderer &engine) const
{
  m_statusBar->render(engine);
  m_logoutConfirmation->render(engine);
}

void StatusUiHandler::updateUi()
{
  m_logoutConfirmation->setVisible(m_logoutRequested);
}

void StatusUiHandler::generateLogoutButton(const pge::Vec2i & /*dimensions*/)
{
  const ui::MenuConfig config{.clickCallback = [this]() { onLogoutRequested(); }};
  const auto bg   = ui::bgConfigFromColor(pge::colors::VERY_DARK_GREY);
  const auto text = ui::textConfigFromColor("Logout", pge::colors::BLACK, pge::colors::RED);

  auto logout = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_statusBar->addMenu(std::move(logout));
}

void StatusUiHandler::generateLogoutConfirmationPanel(const pge::Vec2i &dimensions)
{
  auto innerPanel = generateBlankHorizontalMenu();
  innerPanel->addMenu(generateSpacer());

  auto bg = ui::bgConfigFromColor(pge::colors::VERY_DARK_RED);
  ui::MenuConfig config{.clickCallback = [this]() { onLogoutConfirmed(); }};
  auto text   = ui::textConfigFromColor("Yes", pge::colors::DARK_RED);
  auto button = std::make_unique<ui::UiTextMenu>(config, bg, text);
  innerPanel->addMenu(std::move(button));

  innerPanel->addMenu(generateSpacer());

  bg                   = ui::bgConfigFromColor(pge::colors::VERY_DARK_GREEN);
  config.clickCallback = [this]() { onLogoutCanceled(); };
  text                 = ui::textConfigFromColor("No", pge::colors::DARK_GREEN);
  button               = std::make_unique<ui::UiTextMenu>(config, bg, text);
  innerPanel->addMenu(std::move(button));

  innerPanel->addMenu(generateSpacer());

  const pge::Vec2i logoutDims{dimensions.x / 3, dimensions.y / 4};
  const pge::Vec2i logoutPos{(dimensions.x - logoutDims.x) / 2, (dimensions.y - logoutDims.y) / 2};

  config = ui::MenuConfig{
    .pos  = logoutPos,
    .dims = logoutDims,
    // Not visible by default
    .visible = false,
  };
  bg                   = ui::bgConfigFromColor(almostOpaque(pge::colors::BLACK));
  m_logoutConfirmation = std::make_unique<ui::UiMenu>(config, bg);

  m_logoutConfirmation->addMenu(generateSpacer());

  config.highlightable = false;
  text       = ui::textConfigFromColor("Do you really want to logout?", pge::colors::WHITE);
  auto label = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_logoutConfirmation->addMenu(std::move(label));

  m_logoutConfirmation->addMenu(std::move(innerPanel));
  m_logoutConfirmation->addMenu(generateSpacer());
}

void StatusUiHandler::onLogoutRequested()
{
  m_logoutRequested = true;
}

void StatusUiHandler::onLogoutConfirmed()
{
  m_queue->pushEvent(std::make_unique<LogoutCommand>());
}

void StatusUiHandler::onLogoutCanceled()
{
  m_logoutRequested = false;
}

} // namespace bsgalone::client
