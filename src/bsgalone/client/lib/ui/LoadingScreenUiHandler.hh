
#pragma once

#include "AbstractUiHandler.hh"
#include "TimeUtils.hh"
#include "UiTextMenu.hh"
#include <optional>

namespace bsgalone::client {

class LoadingScreenUiHandler : public AbstractUiHandler
{
  public:
  LoadingScreenUiHandler(const Views &views);
  ~LoadingScreenUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  ui::UiTextMenuPtr m_loadingText{};
  int m_dotCount{0};
  std::optional<::core::TimeStamp> m_lastTextUpdate{};
};

} // namespace bsgalone::client
