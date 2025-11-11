
#include "LoadingScreenUiHandler.hh"
#include "ScreenCommon.hh"
#include "UiTextMenu.hh"

namespace pge {

LoadingScreenUiHandler::LoadingScreenUiHandler(const Views & /*views*/)
  : IUiHandler("loading")
{}

void LoadingScreenUiHandler::initializeMenus(const int width,
                                             const int height,
                                             sprites::TexturePack & /*texturesLoader*/)
{
  const Vec2i loadingTextDimsPixels{100, 30};
  const Vec2i loadingTextPos{(width - loadingTextDimsPixels.x) / 2,
                             (height - loadingTextDimsPixels.y) / 2};
  const MenuConfig config{
    .pos  = loadingTextPos,
    .dims = loadingTextDimsPixels,
  };

  const auto bg   = bgConfigFromColor(colors::TRANSPARENT_WHITE);
  const auto text = textConfigFromColor("Loading", colors::WHITE);

  m_loadingText = std::make_unique<UiTextMenu>(config, bg, text);
}

bool LoadingScreenUiHandler::processUserInput(UserInputData & /*inputData*/)
{
  return false;
}

void LoadingScreenUiHandler::render(Renderer &engine) const
{
  m_loadingText->render(engine);
}

void LoadingScreenUiHandler::updateUi()
{
  if (!m_lastTextUpdate)
  {
    m_lastTextUpdate = core::now();
    return;
  }

  const auto now     = core::now();
  const auto elapsed = now - *m_lastTextUpdate;

  const auto TEXT_UPDATE_INTERVAL = core::fromMilliseconds(250);
  if (elapsed < TEXT_UPDATE_INTERVAL)
  {
    return;
  }

  constexpr auto DOT_COUNT_RESET = 4;

  m_lastTextUpdate = now;
  m_dotCount       = (m_dotCount + 1) % DOT_COUNT_RESET;

  const auto text = "Loading" + std::string(m_dotCount, '.');
  m_loadingText->setText(text);
}

} // namespace pge
