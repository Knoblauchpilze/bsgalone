
#pragma once

#include "AbstractUiHandler.hh"
#include "UiTextMenu.hh"
#include "Views.hh"

namespace bsgalone::client {

class GameScreenUiHandler : public AbstractUiHandler
{
  public:
  GameScreenUiHandler(const Views &views);
  ~GameScreenUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;
  void connectToMessageQueue(core::IMessageQueue &messageQueue) override;

  private:
  std::vector<AbstractUiHandlerPtr> m_uis{};

  void initializeUis(const Views &views);
};

} // namespace bsgalone::client
