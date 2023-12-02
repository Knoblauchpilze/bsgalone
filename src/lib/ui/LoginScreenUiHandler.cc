
#include "LoginScreenUiHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

LoginScreenUiHandler::LoginScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("login")
  , m_loginView(views.loginView)
{
  if (nullptr == m_loginView)
  {
    throw std::invalid_argument("Expected non null login view");
  }
}

void LoginScreenUiHandler::initializeMenus(const int width, const int height)
{
  generateLoginMenu(width, height);
  generateLoginButton(width, height);
  generateQuitButton(width, height);
}

bool LoginScreenUiHandler::processUserInput(UserInputData &inputData)
{
  if (m_quitButton->processUserInput(inputData))
  {
    return true;
  }
  if (m_loginButton->processUserInput(inputData))
  {
    return true;
  }

  return m_loginPanel->processUserInput(inputData);
}

void LoginScreenUiHandler::render(SpriteRenderer &engine) const
{
  m_loginPanel->render(engine.getRenderer());
  m_loginButton->render(engine.getRenderer());
  m_quitButton->render(engine.getRenderer());
}

void LoginScreenUiHandler::updateUi() {}

namespace {
constexpr auto DUMMY_PIXEL_DIMENSION = 10;
const olc::vi2d DUMMY_DIMENSION{DUMMY_PIXEL_DIMENSION, DUMMY_PIXEL_DIMENSION};

void addLoginTitleToMenu(UiMenu &mainPanel)
{
  const MenuConfig config{.dims = DUMMY_DIMENSION, .highlightable = false};

  auto bg   = bgConfigFromColor(transparent(olc::DARK_BLUE, alpha::SemiOpaque));
  auto text = textConfigFromColor("Login", olc::GREY, TextAlignment::CENTER);
  auto menu = std::make_unique<UiTextMenu>(config, bg, text);

  mainPanel.addMenu(std::move(menu));
}

auto addTextFieldSectionToMenu(UiMenu &mainPanel,
                               const std::string &textFieldlabel,
                               const std::optional<std::string> &defaultValue = {}) -> UiTextField *
{
  auto textFieldSection = generateBlankHorizontalMenu();

  const MenuConfig config{.dims = DUMMY_DIMENSION, .highlightable = false};
  auto bg    = bgConfigFromColor(transparent(olc::DARK_BLUE, alpha::SemiOpaque));
  auto text  = textConfigFromColor(textFieldlabel, olc::GREY, TextAlignment::RIGHT);
  auto label = std::make_unique<UiTextMenu>(config, bg, text);
  textFieldSection->addMenu(std::move(label));

  TextFieldConfig fieldConfig{.dims = DUMMY_DIMENSION};
  bg               = bgConfigFromColor(transparent(olc::WHITE, alpha::SemiOpaque));
  const auto value = defaultValue.value_or("");
  text             = textConfigFromColor(value, olc::BLACK, TextAlignment::LEFT);

  auto field = std::make_unique<UiTextField>(fieldConfig, bg, text);
  auto out   = field.get();
  textFieldSection->addMenu(std::move(field));

  mainPanel.addMenu(std::move(textFieldSection));

  return out;
}
} // namespace

void LoginScreenUiHandler::generateLoginMenu(const int width, const int /*height*/)
{
  const olc::vi2d loginDimsPixels{400, 400};
  constexpr auto LOGIN_PANEL_Y_PIXELS = 30;
  const olc::vi2d loginPos{(width - loginDimsPixels.x) / 2, LOGIN_PANEL_Y_PIXELS};

  MenuConfig config{.pos = loginPos, .dims = loginDimsPixels, .highlightable = false};
  m_loginPanel = generateBlankVerticalMenu(loginPos, loginDimsPixels);

  addLoginTitleToMenu(*m_loginPanel);
  m_loginPanel->addMenu(generateColoredSpacer(transparent(olc::DARK_BLUE, alpha::SemiOpaque)));
  m_nameTextField     = addTextFieldSectionToMenu(*m_loginPanel, "Name:", "grouton");
  m_passwordTextField = addTextFieldSectionToMenu(*m_loginPanel, "Password:", {});
}

void LoginScreenUiHandler::generateLoginButton(const int width, const int height)
{
  constexpr auto REASONABLE_GAP_SIZE = 60;

  const olc::vi2d loginButtonDimsPixels{200, 60};
  const olc::vi2d loginButtonPos{(width - loginButtonDimsPixels.x) / 2,
                                 height - REASONABLE_GAP_SIZE - loginButtonDimsPixels.y};

  const MenuConfig config{.pos               = loginButtonPos,
                          .dims              = loginButtonDimsPixels,
                          .gameClickCallback = [this](Game &g) { tryLogin(g); }};

  const auto bg   = bgConfigFromColor(olc::DARK_COBALT_BLUE);
  const auto text = textConfigFromColor("AU JEU!!", olc::WHITE);
  m_loginButton   = std::make_unique<UiTextMenu>(config, bg, text);
}

void LoginScreenUiHandler::generateQuitButton(const int width, const int /*height*/)
{
  constexpr auto REASONABLE_GAP_SIZE = 20;
  const olc::vi2d quitButtonDimsPixels{100, 50};
  const olc::vi2d quitButtonPos{width - REASONABLE_GAP_SIZE - quitButtonDimsPixels.x,
                                REASONABLE_GAP_SIZE};

  const MenuConfig config{.pos               = quitButtonPos,
                          .dims              = quitButtonDimsPixels,
                          .gameClickCallback = [](Game &g) {
                            g.setScreen(Screen::EXIT);
                            g.terminate();
                          }};

  const auto bg   = bgConfigFromColor(olc::VERY_DARK_GREY);
  const auto text = textConfigFromColor("Quit", olc::WHITE);
  m_quitButton    = std::make_unique<UiTextMenu>(config, bg, text);
}

void LoginScreenUiHandler::tryLogin(Game &game)
{
  const auto name     = m_nameTextField->getText();
  const auto password = m_passwordTextField->getText();

  if (name.empty() || password.empty())
  {
    warn("Can't login with empty name or password");
    return;
  }

  const auto playerId = m_loginView->tryLogin(name, password);
  if (playerId)
  {
    game.login(*playerId);
  }
}

} // namespace pge
