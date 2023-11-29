
#pragma once

#include "IUiHandler.hh"
#include "UiMenu.hh"

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
  bsgo::ServerViewShPtr m_serverView;

  std::vector<UiMenuPtr> m_buttons{};

  void generateQuitButton(const int width, const int height);
  void generateMap(const int width, const int height);
  void generateSystemButtons(const bsgo::System &system,
                             const bsgo::ServerView::Bounds &bounds,
                             const olc::vi2d &mapOffset,
                             const olc::vi2d &mapDims);
};

} // namespace pge
