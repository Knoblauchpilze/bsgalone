
#pragma once

#include "AbstractUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>
#include <optional>

namespace bsgalone::client {

class HangarUiHandler : public AbstractUiHandler
{
  public:
  HangarUiHandler(const Views &views);
  ~HangarUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  GameViewShPtr m_gameView{};
  bool m_initialized{false};

  ui::UiMenuPtr m_resourcesMenu{};
  ui::UiMenuPtr m_menu{};

  enum class State
  {
    TO_BUY,
    UNAFFORDABLE,
    TO_EQUIP,
    EQUIPED
  };

  struct PlayerShipData
  {
    core::Uuid shipDbId{};
    std::optional<core::Uuid> playerShipDbId{};
    ui::UiMenu *menu{};
    ui::UiTextMenu *button{};
    State state{State::UNAFFORDABLE};
  };
  std::vector<PlayerShipData> m_shipsData{};

  void subscribeToViews();
  void reset();

  void initializeHangar();
  void generateResourcesMenus();
  void initializeLayout();

  void updateShipMenus();

  void onShipRequest(const int shipIndex);
  void onPurchaseRequest(const int shipIndex);
  void onSelectRequest(const int shipIndex);
};

using HangarUiHandlerPtr = std::unique_ptr<HangarUiHandler>;

} // namespace bsgalone::client
