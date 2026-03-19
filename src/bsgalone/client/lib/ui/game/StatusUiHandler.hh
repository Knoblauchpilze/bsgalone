
#pragma once

#include "AbstractUiHandler.hh"
#include "UiTextMenu.hh"
#include <memory>

namespace bsgalone::client {

class StatusUiHandler : public AbstractUiHandler
{
  public:
  StatusUiHandler(const pge::Vec2i &offset, const Views &views);
  ~StatusUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  pge::Vec2i m_offset{};
  GameViewShPtr m_gameView{};

  bool m_logoutRequested{false};

  ui::UiMenuPtr m_statusBar{};
  ui::UiTextMenu *m_system{};
  ui::UiMenuPtr m_logoutConfirmation{};

  void subscribeToViews();
  void reset();
  void generateLogoutButton(const int width, const int height);
  void generateLogoutConfirmationPanel(const int width, const int height);

  void requestLogout();
  void confirmLogout();
  void cancelLogout();
};

using StatusUiHandlerPtr = std::unique_ptr<StatusUiHandler>;

} // namespace bsgalone::client
