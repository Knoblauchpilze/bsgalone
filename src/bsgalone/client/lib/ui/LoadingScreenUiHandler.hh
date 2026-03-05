
#pragma once

#include "AbstractUiHandler.hh"
#include "TimeUtils.hh"
#include "UiTextMenu.hh"
#include <optional>

namespace pge {

class LoadingScreenUiHandler : public AbstractUiHandler
{
  public:
  LoadingScreenUiHandler(const Views &views);
  ~LoadingScreenUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;

  private:
  ui::UiTextMenuPtr m_loadingText{};
  int m_dotCount{0};
  std::optional<core::TimeStamp> m_lastTextUpdate{};
};

} // namespace pge
