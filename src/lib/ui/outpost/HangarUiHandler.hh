
#pragma once

#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class HangarUiHandler : public IUiHandler
{
  public:
  HangarUiHandler(const bsgo::Views &views);
  ~HangarUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::PlayerViewShPtr m_playerView;
  bool m_initialized{false};

  UiMenuPtr m_menu{};
  std::vector<UiMenu *> m_ships{};

  void initializeHangar();
  void initializeLayout();
  void generateShipsMenus();
};

using HangarUiHandlerPtr = std::unique_ptr<HangarUiHandler>;

} // namespace pge
