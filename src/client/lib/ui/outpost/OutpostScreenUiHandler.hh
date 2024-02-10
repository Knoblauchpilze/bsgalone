
#pragma once

#include "AbstractMessageListener.hh"
#include "HangarUiHandler.hh"
#include "IUiHandler.hh"
#include "LockerUiHandler.hh"
#include "Services.hh"
#include "ShopUiHandler.hh"
#include "UiMenu.hh"
#include "Views.hh"

namespace pge {

class OutpostScreenUiHandler : public IUiHandler, public bsgo::AbstractMessageListener
{
  public:
  OutpostScreenUiHandler(const bsgo::Views &views);
  ~OutpostScreenUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void reset() override;

  void connectToMessageQueue(bsgo::IMessageQueue &messageQueue) override;
  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::ShipDbViewShPtr m_shipDbView{};
  bsgo::PlayerViewShPtr m_playerView{};

  enum MenuItem
  {
    UNDOCK     = 0,
    LOGOUT     = 1,
    VIEWS_MENU = 2,

    COUNT = 3,
  };
  std::vector<UiMenuPtr> m_menus{};

  enum class ActiveScreen
  {
    HANGAR,
    LOCKER,
    SHOP
  };

  ActiveScreen m_activeScreen{ActiveScreen::LOCKER};
  bool m_refreshRequested{false};
  LockerUiHandlerPtr m_lockerUi{};
  ShopUiHandlerPtr m_shopUi{};
  HangarUiHandlerPtr m_hangarUi{};

  void generateGeneralMenu(const int width, const int height);
  void setActiveScreen(const ActiveScreen &screen);
};

} // namespace pge
