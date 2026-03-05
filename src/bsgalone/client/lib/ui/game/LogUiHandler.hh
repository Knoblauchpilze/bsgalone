
#pragma once

#include "AbstractMessageListener.hh"
#include "AbstractUiHandler.hh"
#include "UiTimedMenu.hh"
#include "Views.hh"
#include <deque>
#include <memory>

namespace pge {

class LogUiHandler : public AbstractUiHandler, public bsgalone::core::AbstractMessageListener
{
  public:
  LogUiHandler(const Views &views);
  ~LogUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;

  void connectToMessageQueue(bsgalone::core::IMessageQueue &messageQueue) override;
  void onEventReceived(const bsgalone::core::IMessage &message) override;

  private:
  SystemViewShPtr m_systemView{};
  ResourceViewShPtr m_resourceView{};
  ShipViewShPtr m_shipView{};
  Vec2i m_offset{};

  struct LogMessage
  {
    ui::UiTimedMenuPtr menu{};
    ui::UiMenu *rawMenu{};
  };

  std::deque<LogMessage> m_logs{};
  std::vector<ui::UiTimedMenu *> m_logsToTrigger{};

  void subscribeToViews();
  void reset();
  auto createMenuFromMessage(const bsgalone::core::IMessage &message) -> ui::UiMenuPtr;
};

using LogUiHandlerPtr = std::unique_ptr<LogUiHandler>;

} // namespace pge
