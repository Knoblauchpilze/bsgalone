
#include "StatusUiHandler.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

namespace pge {

StatusUiHandler::StatusUiHandler(const olc::vi2d &offset, const bsgo::Views &views)
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
  const olc::vi2d statusMenuDims{width - 2 * m_offset.x, STATUS_MENU_HEIGHT};

  const MenuConfig config{.pos           = m_offset,
                          .dims          = statusMenuDims,
                          .layout        = MenuLayout::HORIZONTAL,
                          .highlightable = false};
  auto bg = bgConfigFromColor(transparent(olc::WHITE, alpha::AlmostTransparent));

  m_statusBar = std::make_unique<UiMenu>(config, bg);

  bg = bgConfigFromColor(olc::BLANK);

  auto text = textConfigFromColor("N/A", olc::BLACK);
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

void StatusUiHandler::render(SpriteRenderer &engine) const
{
  m_statusBar->render(engine.getRenderer());
  m_logoutConfirmation->render(engine.getRenderer());
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

namespace {
constexpr auto DUMMY_PIXEL_DIMENSION = 10;
const olc::vi2d DUMMY_DIMENSION{DUMMY_PIXEL_DIMENSION, DUMMY_PIXEL_DIMENSION};
} // namespace

void StatusUiHandler::generateLogoutButton(const int /*width*/, const int /*height*/)
{
  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .clickCallback = [this]() {
                            requestLogout();
                          }};
  const auto bg   = bgConfigFromColor(olc::VERY_DARK_GREY);
  const auto text = textConfigFromColor("Logout", olc::BLACK, olc::RED);

  auto logout = std::make_unique<UiTextMenu>(config, bg, text);
  m_statusBar->addMenu(std::move(logout));
}

void StatusUiHandler::generateLogoutConfirmationPanel(const int width, const int height)
{
  auto innerPanel = generateBlankHorizontalMenu();
  innerPanel->addMenu(generateSpacer());

  auto bg = bgConfigFromColor(olc::VERY_DARK_RED);
  MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .gameClickCallback = [this](Game &g) {
                      confirmLogout(g);
                    }};
  auto text   = textConfigFromColor("Yes", olc::DARK_RED);
  auto button = std::make_unique<UiTextMenu>(config, bg, text);
  innerPanel->addMenu(std::move(button));

  innerPanel->addMenu(generateSpacer());

  bg                       = bgConfigFromColor(olc::VERY_DARK_GREEN);
  config.gameClickCallback = {};
  config.clickCallback     = [this]() { cancelLogout(); };
  text                     = textConfigFromColor("No", olc::DARK_GREEN);
  button                   = std::make_unique<UiTextMenu>(config, bg, text);
  innerPanel->addMenu(std::move(button));

  innerPanel->addMenu(generateSpacer());

  const olc::vi2d logoutDims{width / 3, height / 4};
  const olc::vi2d logoutPos{(width - logoutDims.x) / 2, (height - logoutDims.y) / 2};

  config               = MenuConfig{.pos = logoutPos, .dims = logoutDims};
  bg                   = bgConfigFromColor(transparent(olc::BLACK, alpha::AlmostOpaque));
  m_logoutConfirmation = std::make_unique<UiMenu>(config, bg);

  m_logoutConfirmation->addMenu(generateSpacer());

  config.highlightable = false;
  text                 = textConfigFromColor("Do you really want to logout?", olc::WHITE);
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
