
#include "GameState.hh"

/// @brief - Ratio of the size of the menus compared
/// to the total size of the window.
#define RATIO_MENU_TO_WINDOW 0.6

namespace {

auto generateDefaultScreen(const olc::vi2d &dims, const olc::Pixel &color) -> pge::MenuShPtr
{
  olc::vi2d size(dims.x * RATIO_MENU_TO_WINDOW, dims.y * RATIO_MENU_TO_WINDOW);
  olc::vi2d pos(dims.x / 2.0f - size.x / 2.0f, dims.y / 2.0f - size.y / 2.0f);

  pge::menu::BackgroundDesc bg  = pge::menu::newColoredBackground(color);
  pge::menu::MenuContentDesc fg = pge::menu::newTextContent("");

  return std::make_shared<pge::Menu>(pos,
                                     size,
                                     "goMenu",
                                     bg,
                                     fg,
                                     pge::menu::Layout::Vertical,
                                     false,
                                     false);
}

auto generateScreenOption(const olc::vi2d &dims,
                          const std::string &text,
                          const olc::Pixel &bgColor,
                          const std::string &name,
                          bool selectable) -> pge::MenuShPtr
{
  pge::menu::BackgroundDesc bg = pge::menu::newColoredBackground(bgColor);
  bg.hColor                    = olc::GREY;

  pge::menu::MenuContentDesc fd = pge::menu::newMenuContent(text, "", dims);
  fd.color                      = olc::WHITE;
  fd.hColor                     = olc::BLACK;
  fd.align                      = pge::menu::Alignment::Center;

  return std::make_shared<pge::Menu>(olc::vi2d(),
                                     dims,
                                     name,
                                     bg,
                                     fd,
                                     pge::menu::Layout::Horizontal,
                                     selectable,
                                     false);
}

} // namespace

namespace pge {

GameState::GameState(const olc::vi2d &dims, const Screen &screen)
  : utils::CoreObject("state")
  // Assign a different screen so that we can use the
  // `setScreen` routine to initialize the visibility
  // status of screens.
  , m_screen(screen == Screen::LOGIN ? Screen::GAMEOVER : Screen::LOGIN)
{
  setService("bsgo");

  generateLoginScreen(dims);
  generateMapScreen(dims);
  generateOutpostScreen(dims);
  generateGameOverScreen(dims);

  // Assign the screen, which will handle the visibility
  // update.
  setScreen(screen);
}

auto GameState::getScreen() const noexcept -> Screen
{
  return m_screen;
}

void GameState::setScreen(const Screen &screen)
{
  if (m_screen == screen)
  {
    return;
  }

  // Assign the state.
  m_screen = screen;

  // Update screens' visibility.
  m_login->setVisible(m_screen == Screen::LOGIN);
  m_map->setVisible(m_screen == Screen::MAP);
  m_outpost->setVisible(m_screen == Screen::OUTPOST);
  m_gameOver->setVisible(m_screen == Screen::GAMEOVER);
}

void GameState::render(olc::PixelGameEngine *pge) const
{
  m_login->render(pge);
  m_map->render(pge);
  m_outpost->render(pge);
  m_gameOver->render(pge);
}

auto GameState::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  menu::InputHandle res{false, false};

  // Propagate the user input to each screen.
  menu::InputHandle cur = m_login->processUserInput(c, actions);
  res.relevant          = (res.relevant || cur.relevant);
  res.selected          = (res.selected || cur.selected);

  cur          = m_map->processUserInput(c, actions);
  res.relevant = (res.relevant || cur.relevant);
  res.selected = (res.selected || cur.selected);

  cur          = m_outpost->processUserInput(c, actions);
  res.relevant = (res.relevant || cur.relevant);
  res.selected = (res.selected || cur.selected);

  cur          = m_gameOver->processUserInput(c, actions);
  res.relevant = (res.relevant || cur.relevant);
  res.selected = (res.selected || cur.selected);

  return res;
}

void GameState::generateLoginScreen(const olc::vi2d &dims)
{
  m_login = generateDefaultScreen(dims, olc::DARK_PINK);

  MenuShPtr m = generateScreenOption(dims, "Login", olc::VERY_DARK_PINK, "login", true);
  m->setSimpleAction([this](Game & /*g*/) { setScreen(Screen::GAME); });
  m_login->addMenu(m);

  m = generateScreenOption(dims, "Quit", olc::VERY_DARK_PINK, "quit", true);
  m->setSimpleAction([this](Game &g) {
    setScreen(Screen::EXIT);
    g.terminate();
  });
  m_login->addMenu(m);
}

void GameState::generateMapScreen(const olc::vi2d &dims)
{
  m_map = generateDefaultScreen(dims, olc::DARK_ORANGE);

  MenuShPtr m = generateScreenOption(dims,
                                     "Back to game",
                                     olc::VERY_DARK_ORANGE,
                                     "back_to_game",
                                     true);
  m->setSimpleAction([this](Game & /*g*/) { setScreen(Screen::GAME); });
  m_map->addMenu(m);
}

void GameState::generateOutpostScreen(const olc::vi2d &dims)
{
  m_outpost = generateDefaultScreen(dims, olc::DARK_YELLOW);

  MenuShPtr m = generateScreenOption(dims,
                                     "Back to game",
                                     olc::VERY_DARK_YELLOW,
                                     "back_to_game",
                                     true);
  m->setSimpleAction([this](Game & /*g*/) { setScreen(Screen::GAME); });
  m_outpost->addMenu(m);
}

void GameState::generateGameOverScreen(const olc::vi2d &dims)
{
  // Generate the main screen.
  m_gameOver = generateDefaultScreen(dims, olc::DARK_MAGENTA);

  MenuShPtr m = generateScreenOption(dims,
                                     "Back to outpost",
                                     olc::VERY_DARK_MAGENTA,
                                     "back_to_op",
                                     true);
  m->setSimpleAction([this](Game & /*g*/) { setScreen(Screen::OUTPOST); });
  m_gameOver->addMenu(m);

  m = generateScreenOption(dims, "Logout", olc::VERY_DARK_MAGENTA, "logout", true);
  m->setSimpleAction([this](Game &g) {
    setScreen(Screen::EXIT);
    g.terminate();
  });
  m_gameOver->addMenu(m);
}

} // namespace pge
