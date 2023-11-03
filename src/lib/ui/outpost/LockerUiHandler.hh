
#pragma once

#include "IUiHandler.hh"
#include "PlayerView.hh"
#include "Views.hh"
#include <memory>
#include <vector>

namespace pge {

class LockerUiHandler : public IUiHandler
{
  public:
  LockerUiHandler(const bsgo::Views &views);
  virtual ~LockerUiHandler() = default;

  void initializeMenus(const int width, const int height) override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::PlayerViewShPtr m_playerView;

  MenuShPtr m_menu{};
  std::vector<MenuShPtr> m_resources{};
  std::vector<MenuShPtr> m_weapons{};
  std::vector<MenuShPtr> m_computers{};

  void initializeLayout();
  void generateResourcesMenus();
  void generateWeaponsMenus();
  void generateComputersMenus();
};

using LockerUiHandlerPtr = std::unique_ptr<LockerUiHandler>;

} // namespace pge
