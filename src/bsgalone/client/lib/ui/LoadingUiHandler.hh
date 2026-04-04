
#pragma once

#include "IUiHandler.hh"
#include "TimeUtils.hh"
#include "UiTextMenu.hh"
#include <optional>

namespace bsgalone::client {

class LoadingUiHandler : public IUiHandler
{
  public:
  LoadingUiHandler()           = default;
  ~LoadingUiHandler() override = default;

  void initializeMenus(const pge::Vec2i &dimensions,
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
