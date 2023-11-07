
#pragma once

#include "HangarUiHandler.hh"
#include "IUiHandler.hh"
#include "LockerUiHandler.hh"
#include "Menu.hh"
#include "ShopUiHandler.hh"
#include "Views.hh"

namespace pge {

class OutpostScreenUiHandler : public IUiHandler
{
  public:
  OutpostScreenUiHandler(const bsgo::Views &views);
  ~OutpostScreenUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::ShipViewShPtr m_shipView;

  enum MenuItem
  {
    UNDOCK     = 0,
    VIEWS_MENU = 1,

    COUNT = 2,
  };
  std::vector<MenuShPtr> m_menus{};

  enum class ActiveScreen
  {
    HANGAR,
    LOCKER,
    SHOP
  };

  ActiveScreen m_activeScreen{ActiveScreen::SHOP};
  LockerUiHandlerPtr m_lockerUi{};
  ShopUiHandlerPtr m_shopUi{};
  HangarUiHandlerPtr m_hangarUi{};

  void generateGeneralMenu(const int width, const int height);
  void setActiveScreen(const ActiveScreen &screen);
};

} // namespace pge
