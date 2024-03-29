
#pragma once

#include "IUiHandler.hh"
#include "UiTextMenu.hh"
#include <memory>

namespace pge {

class StatusUiHandler : public IUiHandler
{
  public:
  StatusUiHandler(const Vec2i &offset, const bsgo::Views &views);
  ~StatusUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
  Vec2i m_offset{};
  bsgo::ShipViewShPtr m_shipView{};
  bsgo::ServerViewShPtr m_serverView{};
  bsgo::PlayerViewShPtr m_playerView{};

  bool m_logoutRequested{false};

  UiMenuPtr m_statusBar{};
  UiTextMenu *m_system{};
  UiMenuPtr m_logoutConfirmation{};

  void generateLogoutButton(const int width, const int height);
  void generateLogoutConfirmationPanel(const int width, const int height);

  void requestLogout();
  void confirmLogout();
  void cancelLogout();
};

using StatusUiHandlerPtr = std::unique_ptr<StatusUiHandler>;

} // namespace pge
