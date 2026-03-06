
#pragma once

#include "AbstractMessageListener.hh"
#include "AbstractUiHandler.hh"
#include "ComputerSlotComponent.hh"
#include "UiMenu.hh"
#include "UiPictureMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>

namespace bsgalone::client {

class AbilitiesUiHandler : public AbstractUiHandler, public core::AbstractMessageListener
{
  public:
  AbilitiesUiHandler(const Views &views);
  ~AbilitiesUiHandler() override = default;

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

  pge::sprites::PackId m_computerTexturesPackId{};
  std::vector<ui::UiPictureMenuPtr> m_computers{};
  std::vector<ui::UiTextMenu *> m_statuses{};

  void subscribeToViews();
  void reset();
  void generateComputersMenus(int width, int height);
  void initializeAbilities();
  void updateComputerMenu(const core::ComputerSlotComponent &computer, const int id);
};

using AbilitiesUiHandlerPtr = std::unique_ptr<AbilitiesUiHandler>;

} // namespace bsgalone::client
