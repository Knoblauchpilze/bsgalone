
#pragma once

#include "IUiHandler.hh"
#include "Menu.hh"

namespace pge {

class OutpostScreenUiHandler : public IUiHandler
{
  public:
  OutpostScreenUiHandler();
  virtual ~OutpostScreenUiHandler() = default;

  void initializeMenus(const int width, const int height) override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  enum MenuItem
  {
    UNDOCK     = 0,
    VIEWS_MENU = 1,

    SHOP   = 2,
    LOCKER = 3,
    HANGAR = 4,

    COUNT = 5,
  };
  std::vector<MenuShPtr> m_menus{};

  void generateGeneralMenu(const int width, const int height);
  void generateShopView(const int width, const int height);
  void generateLockerView(const int width, const int height);
  void generateHangarView(const int width, const int height);
};

} // namespace pge
