
#pragma once

#include "AbstractUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>
#include <optional>

namespace bsgalone::client {

class GameRoleUiHandler : public AbstractUiHandler
{
  public:
  GameRoleUiHandler(const Views &views);
  ~GameRoleUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  PlayerViewShPtr m_playerView{};
  SystemViewShPtr m_systemView{};
  bool m_initialized{false};

  ui::UiMenuPtr m_menu{};

  enum class State
  {
    TO_JOIN,
    JOINED
  };

  struct PlayerShipData
  {
    core::Uuid shipDbId{};
    core::Uuid playerDbId{};
    ui::UiMenu *menu{};
    ui::UiTextMenu *button{};
    State state{State::TO_JOIN};
  };
  std::vector<PlayerShipData> m_shipsData{};

  void subscribeToViews();
  void reset();

  void initializeAvailableShips();
  void initializeLayout();

  void updateShipMenus();

  void onShipRequest(const int shipIndex);
  void onJoinRequest(const int shipIndex);
};

using GameRoleUiHandlerPtr = std::unique_ptr<GameRoleUiHandler>;

} // namespace bsgalone::client
