
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
  WeaponsUiHandler(const bsgo::Views &views);
  ~WeaponsUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void reset() override;
  void connectToMessageQueue(bsgo::IMessageQueue &messageQueue) override;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::ShipViewShPtr m_shipView{};
  bsgo::ShipDbViewShPtr m_shipDbView{};
  bsgo::PlayerViewShPtr m_playerView{};
  bool m_initialized{false};
  bool m_disabled{false};

  std::vector<UiPictureMenuPtr> m_weapons{};
  std::vector<UiTextMenu *> m_ranges{};
  std::vector<UiTextMenu *> m_damages{};
  std::vector<UiTextMenu *> m_statuses{};

  void generateWeaponsMenus(int width, int height);
  void initializeWeapons();
  void updateWeaponMenu(const bsgo::WeaponSlotComponent &weapon, const int id);
};

using WeaponsUiHandlerPtr = std::unique_ptr<WeaponsUiHandler>;

} // namespace pge
