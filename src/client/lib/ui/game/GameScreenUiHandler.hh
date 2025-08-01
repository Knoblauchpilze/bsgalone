
#pragma once

#include "IUiHandler.hh"
#include "UiTextMenu.hh"
#include "Views.hh"

namespace pge {

class GameScreenUiHandler : public IUiHandler
{
  public:
  GameScreenUiHandler(const Views &views);
  ~GameScreenUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void onLoadingTransition(const bsgo::LoadingTransition transition) override;
  void connectToMessageQueue(bsgo::IMessageQueue &messageQueue) override;

  private:
  std::vector<IUiHandlerPtr> m_uis{};

  void initializeUis(const Views &views);
};

} // namespace pge
