
#include "LoadingScreenUiHandler.hh"
#include "ScreenCommon.hh"
#include "UiTextMenu.hh"

namespace bsgalone::client {

LoadingScreenUiHandler::LoadingScreenUiHandler(const Views & /*views*/)
  : AbstractUiHandler("loading")
{}

void LoadingScreenUiHandler::initializeMenus(const int width,
                                             const int height,
                                             pge::sprites::TexturePack & /*texturesLoader*/)
{
  const pge::Vec2i loadingTextDimsPixels{100, 30};
  const pge::Vec2i loadingTextPos{(width - loadingTextDimsPixels.x) / 2,
                                  (height - loadingTextDimsPixels.y) / 2};
  const ui::MenuConfig config{
    .pos  = loadingTextPos,
    .dims = loadingTextDimsPixels,
  };

  const auto bg   = ui::bgConfigFromColor(pge::colors::TRANSPARENT_WHITE);
  const auto text = ui::textConfigFromColor("Loading", pge::colors::WHITE);

  m_loadingText = std::make_unique<ui::UiTextMenu>(config, bg, text);
}

bool LoadingScreenUiHandler::processUserInput(ui::UserInputData & /*inputData*/)
{
  return false;
}

void LoadingScreenUiHandler::render(pge::Renderer &engine) const
{
  m_loadingText->render(engine);
}

void LoadingScreenUiHandler::updateUi()
{
  if (!m_lastTextUpdate)
  {
    m_lastTextUpdate = ::core::now();
    return;
  }

  const auto now     = ::core::now();
  const auto elapsed = now - *m_lastTextUpdate;

  const auto TEXT_UPDATE_INTERVAL = ::core::fromMilliseconds(250);
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

} // namespace bsgalone::client
