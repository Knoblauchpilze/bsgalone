
#pragma once

#include "IUiHandler.hh"
#include "UiMenu.hh"
#include <optional>
#include <unordered_map>

namespace pge {

class MapScreenUiHandler : public IUiHandler
{
  public:
  MapScreenUiHandler(const Views &views);
  ~MapScreenUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
  ServerViewShPtr m_serverView{};
  ShipViewShPtr m_shipView{};
  ShipDbViewShPtr m_shipDbView{};

  std::vector<UiMenuPtr> m_buttons{};
  UiMenu *m_jumpButton{};
  std::unordered_map<bsgo::Uuid, UiMenu *> m_systemMenus{};

  struct SelectedSystem
  {
    bsgo::Uuid systemId{};
    int labelId{};
  };
  std::optional<SelectedSystem> m_selectedSystem{};

  void generateControlButtons(const int width, const int height);
  void generateMap(const int width, const int height);
  void generateSystemButtons(const bsgo::System &system,
                             const ServerView::Bounds &bounds,
                             const Vec2i &mapOffset,
                             const Vec2i &mapDims);

  void onSystemSelected(const bsgo::Uuid systemId, const int labelId);
  void onJumpRequested();
};

} // namespace pge
