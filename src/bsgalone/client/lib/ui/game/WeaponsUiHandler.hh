
#pragma once

#include "AbstractMessageListener.hh"
#include "AbstractUiHandler.hh"
#include "UiMenu.hh"
#include "UiPictureMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include "WeaponSlotComponent.hh"
#include <memory>

namespace bsgalone::client {

class WeaponsUiHandler : public AbstractUiHandler, public core::AbstractMessageListener
{
  public:
  WeaponsUiHandler(const Views &views);
  ~WeaponsUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;
  void connectToMessageQueue(core::IMessageQueue &messageQueue) override;

  void onEventReceived(const core::IMessage &message) override;

  private:
  ShipViewShPtr m_shipView{};
  ShipDbViewShPtr m_shipDbView{};
  PlayerViewShPtr m_playerView{};
  bool m_initialized{false};
  bool m_disabled{false};

  pge::sprites::PackId m_weaponTexturesPackId{};
  std::vector<ui::UiPictureMenuPtr> m_weapons{};
  std::vector<ui::UiTextMenu *> m_statuses{};

  void subscribeToViews();
  void reset();
  void generateWeaponsMenus(int width, int height);
  void initializeWeapons();
  void updateWeaponMenu(const core::WeaponSlotComponent &weapon, const int id);
};

using WeaponsUiHandlerPtr = std::unique_ptr<WeaponsUiHandler>;

} // namespace bsgalone::client
