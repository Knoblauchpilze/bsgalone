
#pragma once

#include "IUiCommandQueue.hh"
#include "IUiHandler.hh"
#include "UiTextMenu.hh"

namespace bsgalone::client {

class StatusUiHandler : public IUiHandler
{
  public:
  explicit StatusUiHandler(IUiCommandQueueShPtr outputQueue);
  ~StatusUiHandler() override = default;

  void initializeMenus(const pge::Vec2i &dimensions,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  bool m_logoutRequested{false};

  ui::UiMenuPtr m_statusBar{};
  ui::UiMenuPtr m_logoutConfirmation{};

  IUiCommandQueueShPtr m_queue{};

  void generateLogoutButton(const pge::Vec2i &dimensions);
  void generateLogoutConfirmationPanel(const pge::Vec2i &dimensions);

  void onLogoutRequested();
  void onLogoutConfirmed();
  void onLogoutCanceled();
};

} // namespace bsgalone::client
