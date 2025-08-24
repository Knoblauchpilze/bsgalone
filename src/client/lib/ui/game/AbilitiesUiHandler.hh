
#pragma once

#include "AbstractMessageListener.hh"
#include "ComputerSlotComponent.hh"
#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiPictureMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class AbilitiesUiHandler : public IUiHandler, public bsgo::AbstractMessageListener
{
  public:
  AbilitiesUiHandler(const Views &views);
  ~AbilitiesUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void reset() override;
  void connectToMessageQueue(bsgo::IMessageQueue &messageQueue) override;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  ShipViewShPtr m_shipView{};
  ShipDbViewShPtr m_shipDbView{};
  PlayerViewShPtr m_playerView{};
  bool m_initialized{false};
  bool m_disabled{false};

  sprites::PackId m_computerTexturesPackId{};
  std::vector<UiPictureMenuPtr> m_computers{};
  std::vector<UiTextMenu *> m_statuses{};

  void subscribeToViews();
  void generateComputersMenus(int width, int height);
  void initializeAbilities();
  void updateComputerMenu(const bsgo::ComputerSlotComponent &computer, const int id);
};

using AbilitiesUiHandlerPtr = std::unique_ptr<AbilitiesUiHandler>;

} // namespace pge
