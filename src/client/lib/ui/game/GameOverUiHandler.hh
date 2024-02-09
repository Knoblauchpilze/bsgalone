
#pragma once

#include "AbstractMessageListener.hh"
#include "IUiHandler.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class GameOverUiHandler : public IUiHandler, public bsgo::AbstractMessageListener
{
  public:
  GameOverUiHandler(const bsgo::Views &views);
  ~GameOverUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void reset() override;
  void connectToMessageQueue(bsgo::IMessageQueue &messageQueue) override;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::ShipDbViewShPtr m_shipDbView{};

  UiTextMenuPtr m_menu{};
};

using GameOverUiHandlerPtr = std::unique_ptr<GameOverUiHandler>;

} // namespace pge
