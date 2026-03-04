
#pragma once

#include "AbstractUiHandler.hh"
#include "UiTextMenu.hh"
#include "Views.hh"

namespace pge {

class GameScreenUiHandler : public AbstractUiHandler
{
  public:
  GameScreenUiHandler(const Views &views);
  ~GameScreenUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void connectToMessageQueue(bsgalone::core::IMessageQueue &messageQueue) override;

  private:
  std::vector<AbstractUiHandlerPtr> m_uis{};

  void initializeUis(const Views &views);
};

} // namespace pge
