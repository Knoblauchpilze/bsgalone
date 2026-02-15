
#pragma once

#include "AbstractMessageListener.hh"
#include "IUiHandler.hh"
#include "TimeUtils.hh"
#include "UiBlinkingMenu.hh"
#include "UiTextMenu.hh"
#include <memory>

namespace pge {

class ShipStatusUiHandler : public IUiHandler, public bsgo::AbstractMessageListener
{
  public:
  ShipStatusUiHandler(const Views &views);
  ~ShipStatusUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;

  void connectToMessageQueue(bsgalone::core::IMessageQueue &messageQueue) override;
  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  ShipViewShPtr m_shipView{};

  UiBlinkingMenuPtr m_threatLabel{};

  UiMenuPtr m_jumpPanel{};
  UiTextMenu *m_jumpDestination{};
  UiTextMenu *m_jumpTime{};

  std::optional<core::TimeStamp> m_jumpStartTime{};

  void subscribeToViews();
  void reset();
  void initializeThreatPanel(const int width, const int height);
  void initializeJumpPanel(const int width, const int height);

  void updateThreatPanel();
  void updateJumpPanel();
};

using ShipStatusUiHandlerPtr = std::unique_ptr<ShipStatusUiHandler>;

} // namespace pge
