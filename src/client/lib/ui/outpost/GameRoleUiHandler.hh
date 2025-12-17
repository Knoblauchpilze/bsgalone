
#pragma once

#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>
#include <optional>

namespace pge {

class GameRoleUiHandler : public IUiHandler
{
  public:
  GameRoleUiHandler(const Views &views);
  ~GameRoleUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;

  private:
  PlayerViewShPtr m_playerView{};
  SystemViewShPtr m_systemView{};
  bool m_initialized{false};

  UiMenuPtr m_menu{};

  enum class State
  {
    TO_JOIN,
    JOINED
  };

  struct PlayerShipData
  {
    bsgo::Uuid shipDbId{};
    bsgo::Uuid playerDbId{};
    UiMenu *menu{};
    UiTextMenu *button{};
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

} // namespace pge
