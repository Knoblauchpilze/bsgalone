
#pragma once

#include "AbstractUiHandler.hh"
#include "SystemData.hh"
#include "UiMenu.hh"
#include <optional>
#include <unordered_map>

namespace bsgalone::client {

class MapScreenUiHandler : public AbstractUiHandler
{
  public:
  MapScreenUiHandler(const Views &views);
  ~MapScreenUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  ServerViewShPtr m_serverView{};
  ShipViewShPtr m_shipView{};
  ShipDbViewShPtr m_shipDbView{};

  pge::Vec2i m_mapDimensions{};
  bool m_initialized{false};
  std::vector<ui::UiMenuPtr> m_buttons{};
  ui::UiMenu *m_jumpButton{};
  std::unordered_map<core::Uuid, ui::UiMenu *> m_systemMenus{};

  struct SelectedSystem
  {
    core::Uuid systemId{};
    int labelId{};
  };
  std::optional<SelectedSystem> m_selectedSystem{};

  void subscribeToViews();
  void reset();
  void generateControlButtons(const int width, const int height);
  void generateMap();
  void generateSystemButtons(const core::SystemData &system,
                             const ServerView::Bounds &bounds,
                             const pge::Vec2i &mapOffset);

  void onSystemSelected(const core::Uuid systemId, const int labelId);
  void onJumpRequested();
};

} // namespace bsgalone::client
