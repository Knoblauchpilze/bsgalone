
#pragma once

#include "IUiCommandQueue.hh"
#include "IUiEventQueue.hh"
#include "IUiHandler.hh"
#include "UiTextMenu.hh"

namespace bsgalone::client {

class UiEventListenerStatusProxy;

class StatusUiHandler : public IUiHandler
{
  public:
  explicit StatusUiHandler(IUiEventQueueShPtr inputQueue, IUiCommandQueueShPtr outputQueue);
  ~StatusUiHandler() override = default;

  void initializeMenus(const pge::Vec2i &dimensions,
                       pge::sprites::ITexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  bool m_logoutRequested{false};

  ui::UiMenuPtr m_statusBar{};
  ui::UiTextMenu *m_systemName{};
  ui::UiMenuPtr m_logoutConfirmation{};

  IUiCommandQueueShPtr m_queue{};

  void registerToQueue(IUiEventQueueShPtr inputQueue);

  void generateLogoutButton(const pge::Vec2i &dimensions);
  void generateLogoutConfirmationPanel(const pge::Vec2i &dimensions);

  void onLogoutRequested();
  void onLogoutConfirmed();
  void onLogoutCanceled();
  void onGameReady(const std::string &systemName);

  friend class UiEventListenerStatusProxy;
};

} // namespace bsgalone::client
