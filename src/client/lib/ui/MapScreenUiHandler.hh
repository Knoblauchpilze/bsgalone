
#pragma once

#include "IUiHandler.hh"
#include "UiMenu.hh"
#include <optional>
#include <unordered_map>

namespace pge {

class MapScreenUiHandler : public IUiHandler
{
  public:
  MapScreenUiHandler(const bsgo::Views &views);
  ~MapScreenUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::ServerViewShPtr m_serverView{};
  bsgo::ShipViewShPtr m_shipView{};

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
                             const bsgo::ServerView::Bounds &bounds,
                             const olc::vi2d &mapOffset,
                             const olc::vi2d &mapDims);

  void onSystemSelected(const bsgo::Uuid &systemId, const int labelId);
  void onJumpRequested();
};

} // namespace pge
