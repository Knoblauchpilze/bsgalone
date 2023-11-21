
#pragma once

#include "IUiHandler.hh"
#include "UiTextMenu.hh"

namespace pge {

class MapScreenUiHandler : public IUiHandler
{
  public:
  MapScreenUiHandler();
  ~MapScreenUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  UiTextMenuPtr m_quitButton{};

  void generateQuitButton(const int width, const int height);
};

} // namespace pge
