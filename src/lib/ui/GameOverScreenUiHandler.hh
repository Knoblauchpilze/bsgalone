
#pragma once

#include "IUiHandler.hh"
#include "Menu.hh"

namespace pge {

class GameOverScreenUiHandler : public IUiHandler
{
  public:
  GameOverScreenUiHandler();
  virtual ~GameOverScreenUiHandler() = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  MenuShPtr m_menu{};
};

} // namespace pge
