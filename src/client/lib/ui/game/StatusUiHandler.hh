
#pragma once

#include "AbstractUiHandler.hh"
#include "UiTextMenu.hh"
#include <memory>

namespace pge {

class StatusUiHandler : public AbstractUiHandler
{
  public:
  StatusUiHandler(const Vec2i &offset, const Views &views);
  ~StatusUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;

  private:
  Vec2i m_offset{};
  ShipViewShPtr m_shipView{};
  ServerViewShPtr m_serverView{};
  PlayerViewShPtr m_playerView{};

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

} // namespace pge
