
#pragma once

#include "IUiHandler.hh"
#include "UiTextMenu.hh"
#include "Views.hh"

namespace pge {

class GameScreenUiHandler : public IUiHandler
{
  public:
  GameScreenUiHandler(const bsgo::Views &views);
  ~GameScreenUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void reset() override;
  void connectToMessageQueue(bsgo::IMessageQueue &messageQueue) override;

  private:
  std::vector<IUiHandlerPtr> m_uis{};

  void initializeUis(const bsgo::Views &views);
};

} // namespace pge
