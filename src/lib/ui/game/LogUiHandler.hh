
#pragma once

#include "AbstractMessageListener.hh"
#include "IUiHandler.hh"
#include "SystemMessage.hh"
#include "UiTimedMenu.hh"
#include "Views.hh"
#include <deque>
#include <memory>

namespace pge {

class LogUiHandler : public IUiHandler, public bsgo::AbstractMessageListener
{
  public:
  LogUiHandler(const bsgo::Views &views);
  ~LogUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;
  void reset() override;

  void connectToMessageQueue(bsgo::IMessageQueue &messageQueue) override;
  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::SystemViewShPtr m_systemView{};
  bsgo::ResourceViewShPtr m_resourceView{};
  olc::vi2d m_offset{};

  struct LogMessage
  {
    UiTimedMenuPtr menu{};
    UiMenu *rawMenu{};
  };

  std::deque<LogMessage> m_logs{};
  std::vector<UiTimedMenu *> m_logsToTrigger{};

  auto createMenuFromMessage(const bsgo::SystemMessage &message) -> UiMenuPtr;
};

using LogUiHandlerPtr = std::unique_ptr<LogUiHandler>;

} // namespace pge
