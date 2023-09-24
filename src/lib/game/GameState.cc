
#include "GameState.hh"

/// @brief - Ratio of the size of the menus compared
/// to the total size of the window.
#define RATIO_MENU_TO_WINDOW 0.6

namespace {

pge::MenuShPtr generateDefaultScreen(const olc::vi2d &dims, const olc::Pixel &color)
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

pge::MenuShPtr generateScreenOption(const olc::vi2d &dims,
                                    const std::string &text,
                                    const olc::Pixel &bgColor,
                                    const std::string &name,
                                    bool selectable)
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
  , m_screen(screen == Screen::Home ? Screen::Exit : Screen::Home)
  , m_savedGames(10u, "data/saves", "ext")
{
  setService("chess");

  generateHomeScreen(dims);
  generateLoadGameScreen(dims);
  generateGameOverScreen(dims);

  // Assign the screen, which will handle the visibility
  // update.
  setScreen(screen);

  // Connect the slot to receive updates about saved games.
  m_savedGames.onSavedGameSelected.connect_member<GameState>(this, &GameState::onSavedGamePicked);
}

GameState::~GameState()
{
  m_savedGames.onSavedGameSelected.disconnectAll();
}

Screen GameState::getScreen() const noexcept
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
  m_home->setVisible(m_screen == Screen::Home);
  m_loadGame->setVisible(m_screen == Screen::LoadGame);
  m_gameOver->setVisible(m_screen == Screen::GameOver);
}

void GameState::render(olc::PixelGameEngine *pge) const
{
  m_home->render(pge);
  m_loadGame->render(pge);
  m_gameOver->render(pge);
}

menu::InputHandle GameState::processUserInput(const controls::State &c,
                                              std::vector<ActionShPtr> &actions)
{
  menu::InputHandle res{false, false};

  // Propagate the user input to each screen.
  menu::InputHandle cur = m_home->processUserInput(c, actions);
  res.relevant          = (res.relevant || cur.relevant);
  res.selected          = (res.selected || cur.selected);

  cur          = m_loadGame->processUserInput(c, actions);
  res.relevant = (res.relevant || cur.relevant);
  res.selected = (res.selected || cur.selected);

  cur          = m_gameOver->processUserInput(c, actions);
  res.relevant = (res.relevant || cur.relevant);
  res.selected = (res.selected || cur.selected);

  return res;
}

void GameState::onSavedGamePicked(const std::string &game)
{
  info("Picked saved game \"" + game + "\"");
  setScreen(Screen::Game);
}

void GameState::generateHomeScreen(const olc::vi2d &dims)
{
  // Generate the main screen.
  m_home = generateDefaultScreen(dims, olc::DARK_PINK);

  // Add each option to the screen.
  MenuShPtr m = generateScreenOption(dims, "New game", olc::VERY_DARK_PINK, "new_game", true);
  m->setSimpleAction([this](Game & /*g*/) { setScreen(Screen::Game); });
  m_home->addMenu(m);

  m = generateScreenOption(dims, "Load game", olc::VERY_DARK_PINK, "load_game", true);
  m->setSimpleAction([this](Game & /*g*/) {
    // Refresh the saved games list.
    m_savedGames.refresh();
    setScreen(Screen::LoadGame);
  });
  m_home->addMenu(m);

  m = generateScreenOption(dims, "Quit", olc::VERY_DARK_PINK, "quit", true);
  m->setSimpleAction([this](Game &g) {
    setScreen(Screen::Exit);
    g.terminate();
  });
  m_home->addMenu(m);
}

void GameState::generateLoadGameScreen(const olc::vi2d &dims)
{
  // Generate the main screen.
  m_loadGame = generateDefaultScreen(dims, olc::DARK_ORANGE);

  // Add each option to the screen.
  MenuShPtr m
    = generateScreenOption(dims, "Saved games:", olc::VERY_DARK_ORANGE, "saved_games", false);
  m_loadGame->addMenu(m);

  m = generateScreenOption(dims, "Back to main screen", olc::VERY_DARK_ORANGE, "back_to_main", true);
  m->setSimpleAction([this](Game & /*g*/) { setScreen(Screen::Home); });
  m_loadGame->addMenu(m);

  m_savedGames.generate(m_loadGame);
  m_savedGames.refresh();
}

void GameState::generateGameOverScreen(const olc::vi2d &dims)
{
  // Generate the main screen.
  m_gameOver = generateDefaultScreen(dims, olc::DARK_MAGENTA);

  MenuShPtr m = generateScreenOption(dims,
                                     "Back to main screen",
                                     olc::VERY_DARK_MAGENTA,
                                     "back_to_main",
                                     true);
  m->setSimpleAction([this](Game & /*g*/) { setScreen(Screen::Home); });
  m_gameOver->addMenu(m);

  m = generateScreenOption(dims, "Restart", olc::VERY_DARK_MAGENTA, "restart", true);
  m->setSimpleAction([this](Game & /*g*/) { setScreen(Screen::Game); });
  m_gameOver->addMenu(m);

  m = generateScreenOption(dims, "Quit", olc::VERY_DARK_MAGENTA, "quit", true);
  m->setSimpleAction([this](Game &g) {
    setScreen(Screen::Exit);
    g.terminate();
  });
  m_gameOver->addMenu(m);
}

} // namespace pge
