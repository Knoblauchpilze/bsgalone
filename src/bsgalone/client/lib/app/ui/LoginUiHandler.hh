
#pragma once

#include "IUiEventQueue.hh"
#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextField.hh"
#include "UiTextMenu.hh"

namespace bsgalone::client {

class LoginUiHandler : public IUiHandler
{
  public:
  LoginUiHandler(IUiEventQueueShPtr eventQueue);
  ~LoginUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  ui::UiMenuPtr m_credentialsPanel{};

  ui::UiTextField *m_nameTextField{};
  ui::UiTextField *m_passwordTextField{};

  ui::UiTextMenuPtr m_proceedButton{};
  ui::UiTextMenuPtr m_quitButton{};

  IUiEventQueueShPtr m_eventQueue{};

  void generateProceedButton(const int width, const int height);
  void generateQuitButton(const int width, const int height);

  void triggerLoginCommand();
  void triggerExitCommand();
};

} // namespace bsgalone::client
