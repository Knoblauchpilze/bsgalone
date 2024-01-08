
#include "StatusUiHandler.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

namespace pge {

StatusUiHandler::StatusUiHandler(const Vec2i &offset, const bsgo::Views &views)
  : IUiHandler("status")
  , m_offset(offset)
  , m_shipView(views.shipView)
  , m_serverView(views.serverView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_serverView)
  {
    throw std::invalid_argument("Expected non null server view");
  }
}

namespace {
constexpr auto STATUS_MENU_HEIGHT = 20;
} // namespace

void StatusUiHandler::initializeMenus(const int width, const int height)
{
  const Vec2i statusMenuDims{width - 2 * m_offset.x, STATUS_MENU_HEIGHT};

  const MenuConfig config{.pos           = m_offset,
                          .dims          = statusMenuDims,
                          .layout        = MenuLayout::HORIZONTAL,
                          .highlightable = false};
  auto bg = bgConfigFromColor(almostTransparent(colors::WHITE));

  m_statusBar = std::make_unique<UiMenu>(config, bg);

  bg = bgConfigFromColor(colors::BLANK);

  auto text = textConfigFromColor("N/A", colors::BLACK);
  auto menu = std::make_unique<UiTextMenu>(config, bg, text);
  m_system  = menu.get();
  m_statusBar->addMenu(std::move(menu));

  m_statusBar->addMenu(generateSpacer());
  m_statusBar->addMenu(generateSpacer());
  m_statusBar->addMenu(generateSpacer());

  generateLogoutButton(width, height);
  generateLogoutConfirmationPanel(width, height);
}

bool StatusUiHandler::processUserInput(UserInputData &inputData)
{
  if (m_statusBar->processUserInput(inputData))
  {
    return true;
  }
  return m_logoutConfirmation->processUserInput(inputData);
}

void StatusUiHandler::render(Renderer &engine) const
{
  m_statusBar->render(engine);
  m_logoutConfirmation->render(engine);
}

void StatusUiHandler::updateUi()
{
  if (!m_serverView->isReady())
  {
    return;
  }

  m_system->setText(m_serverView->getPlayerSystemName());
  m_logoutConfirmation->setVisible(m_logoutRequested);
}

void StatusUiHandler::reset()
{
  m_logoutRequested = false;
}

void StatusUiHandler::generateLogoutButton(const int /*width*/, const int /*height*/)
{
  const MenuConfig config{.clickCallback = [this]() { requestLogout(); }};
  const auto bg   = bgConfigFromColor(colors::VERY_DARK_GREY);
  const auto text = textConfigFromColor("Logout", colors::BLACK, colors::RED);

  auto logout = std::make_unique<UiTextMenu>(config, bg, text);
  m_statusBar->addMenu(std::move(logout));
}

void StatusUiHandler::generateLogoutConfirmationPanel(const int width, const int height)
{
  auto innerPanel = generateBlankHorizontalMenu();
  innerPanel->addMenu(generateSpacer());

  auto bg = bgConfigFromColor(colors::VERY_DARK_RED);
  MenuConfig config{.gameClickCallback = [this](Game &g) { confirmLogout(g); }};
  auto text   = textConfigFromColor("Yes", colors::DARK_RED);
  auto button = std::make_unique<UiTextMenu>(config, bg, text);
  innerPanel->addMenu(std::move(button));

  innerPanel->addMenu(generateSpacer());

  bg                       = bgConfigFromColor(colors::VERY_DARK_GREEN);
  config.gameClickCallback = {};
  config.clickCallback     = [this]() { cancelLogout(); };
  text                     = textConfigFromColor("No", colors::DARK_GREEN);
  button                   = std::make_unique<UiTextMenu>(config, bg, text);
  innerPanel->addMenu(std::move(button));

  innerPanel->addMenu(generateSpacer());

  const Vec2i logoutDims{width / 3, height / 4};
  const Vec2i logoutPos{(width - logoutDims.x) / 2, (height - logoutDims.y) / 2};

  config               = MenuConfig{.pos = logoutPos, .dims = logoutDims};
  bg                   = bgConfigFromColor(almostOpaque(colors::BLACK));
  m_logoutConfirmation = std::make_unique<UiMenu>(config, bg);

  m_logoutConfirmation->addMenu(generateSpacer());

  config.highlightable = false;
  text                 = textConfigFromColor("Do you really want to logout?", colors::WHITE);
  auto label           = std::make_unique<UiTextMenu>(config, bg, text);
  m_logoutConfirmation->addMenu(std::move(label));

  m_logoutConfirmation->addMenu(std::move(innerPanel));
  m_logoutConfirmation->addMenu(generateSpacer());
}

void StatusUiHandler::requestLogout()
{
  m_logoutRequested = true;
}

void StatusUiHandler::confirmLogout(Game &g)
{
  g.setScreen(Screen::LOGIN);
}

void StatusUiHandler::cancelLogout()
{
  m_logoutRequested = false;
}

} // namespace pge
