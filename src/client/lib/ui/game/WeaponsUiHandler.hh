
#pragma once

#include "AbstractMessageListener.hh"
#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiPictureMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include "WeaponSlotComponent.hh"
#include <memory>

namespace pge {

class WeaponsUiHandler : public IUiHandler, public bsgo::AbstractMessageListener
{
  public:
  WeaponsUiHandler(const Views &views);
  ~WeaponsUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void connectToMessageQueue(bsgo::IMessageQueue &messageQueue) override;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  ShipViewShPtr m_shipView{};
  ShipDbViewShPtr m_shipDbView{};
  PlayerViewShPtr m_playerView{};
  bool m_initialized{false};
  bool m_disabled{false};

  sprites::PackId m_weaponTexturesPackId{};
  std::vector<UiPictureMenuPtr> m_weapons{};
  std::vector<UiTextMenu *> m_statuses{};

  void subscribeToViews();
  void reset();
  void generateWeaponsMenus(int width, int height);
  void initializeWeapons();
  void updateWeaponMenu(const bsgo::WeaponSlotComponent &weapon, const int id);
};

using WeaponsUiHandlerPtr = std::unique_ptr<WeaponsUiHandler>;

} // namespace pge
