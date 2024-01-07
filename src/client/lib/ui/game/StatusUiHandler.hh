
#pragma once

#include "IUiHandler.hh"
#include "UiTextMenu.hh"
#include <memory>

namespace pge {

class StatusUiHandler : public IUiHandler
{
  public:
  StatusUiHandler(const olc::vi2d &offset, const bsgo::Views &views);
  ~StatusUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
  olc::vi2d m_offset{};
  bsgo::ShipViewShPtr m_shipView{};
  bsgo::ServerViewShPtr m_serverView{};

  bool m_logoutRequested{false};

  UiMenuPtr m_statusBar{};
  UiTextMenu *m_system{};
  UiMenuPtr m_logoutConfirmation{};

  void generateLogoutButton(const int width, const int height);
  void generateLogoutConfirmationPanel(const int width, const int height);

  void requestLogout();
  void confirmLogout(Game &g);
  void cancelLogout();
};

using StatusUiHandlerPtr = std::unique_ptr<StatusUiHandler>;

} // namespace pge
