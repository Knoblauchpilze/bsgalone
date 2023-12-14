
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

  m_menu = std::make_unique<UiMenu>(config, bg);

  bg = bgConfigFromColor(olc::BLANK);

  auto text = textConfigFromColor("N/A", olc::BLACK);
  auto menu = std::make_unique<UiTextMenu>(config, bg, text);
  m_system  = menu.get();
  m_menu->addMenu(std::move(menu));

  m_menu->addMenu(generateSpacer());
  m_menu->addMenu(generateSpacer());
  m_menu->addMenu(generateSpacer());

  generateLogoutButton(width, height);
}

bool StatusUiHandler::processUserInput(UserInputData &inputData)
{
  return m_menu->processUserInput(inputData);
}

void StatusUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
}

void StatusUiHandler::updateUi()
{
  if (!m_serverView->isReady())
  {
    return;
  }

  m_system->setText(m_serverView->getPlayerSystemName());
}

namespace {
constexpr auto DUMMY_PIXEL_DIMENSION = 10;
const olc::vi2d DUMMY_DIMENSION{DUMMY_PIXEL_DIMENSION, DUMMY_PIXEL_DIMENSION};
} // namespace

void StatusUiHandler::generateLogoutButton(const int /*width*/, const int /*height*/)
{
  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .gameClickCallback = [](Game &g) {
                            g.setScreen(Screen::LOGIN);
                          }};
  const auto bg   = bgConfigFromColor(olc::VERY_DARK_GREY);
  const auto text = textConfigFromColor("Logout", olc::BLACK, olc::RED);

  auto logout = std::make_unique<UiTextMenu>(config, bg, text);
  m_menu->addMenu(std::move(logout));
}

} // namespace pge
