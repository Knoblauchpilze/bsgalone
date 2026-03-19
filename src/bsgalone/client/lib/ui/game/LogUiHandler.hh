
#pragma once

#include "AbstractMessageListener.hh"
#include "AbstractUiHandler.hh"
#include "UiTimedMenu.hh"
#include "Views.hh"
#include <deque>
#include <memory>

namespace bsgalone::client {

class LogUiHandler : public AbstractUiHandler, public core::AbstractMessageListener
{
  public:
  LogUiHandler(const Views &views);
  ~LogUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  void connectToMessageQueue(core::IMessageQueue &messageQueue) override;
  void onEventReceived(const core::IMessage &message) override;

  private:
  GameViewShPtr m_gameView{};
  pge::Vec2i m_offset{};

  struct LogMessage
  {
    ui::UiTimedMenuPtr menu{};
    ui::UiMenu *rawMenu{};
  };

  std::deque<LogMessage> m_logs{};
  std::vector<ui::UiTimedMenu *> m_logsToTrigger{};

  void subscribeToViews();
  void reset();
  auto createMenuFromMessage(const core::IMessage &message) -> ui::UiMenuPtr;
};

using LogUiHandlerPtr = std::unique_ptr<LogUiHandler>;

} // namespace bsgalone::client
