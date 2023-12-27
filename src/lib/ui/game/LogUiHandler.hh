
#pragma once

#include "AbstractMessageListener.hh"
#include "IUiHandler.hh"
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
  olc::vi2d m_offset{};
  std::deque<UiTimedMenuPtr> m_logs{};

  auto createMenuFromMessage(const bsgo::IMessage &message) -> UiTimedMenuPtr;
};

using LogUiHandlerPtr = std::unique_ptr<LogUiHandler>;

} // namespace pge
