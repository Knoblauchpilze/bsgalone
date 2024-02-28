
#pragma once

#include "AbstractMessageListener.hh"
#include "IUiHandler.hh"
#include "UiBlinkingMenu.hh"
#include "UiTextMenu.hh"
#include <core_utils/TimeUtils.hh>
#include <memory>

namespace pge {

class ShipStatusUiHandler : public IUiHandler, public bsgo::AbstractMessageListener
{
  public:
  ShipStatusUiHandler(const bsgo::Views &views);
  ~ShipStatusUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void reset() override;

  void connectToMessageQueue(bsgo::IMessageQueue &messageQueue) override;
  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::ShipViewShPtr m_shipView{};

  UiBlinkingMenuPtr m_threatLabel{};

  UiMenuPtr m_jumpPanel{};
  UiTextMenu *m_jumpDestination{};
  UiTextMenu *m_jumpTime{};

  std::optional<utils::TimeStamp> m_jumpStartTime{};

  void initializeThreatPanel(const int width, const int height);
  void initializeJumpPanel(const int width, const int height);

  void updateThreatPanel();
  void updateJumpPanel();
};

using ShipStatusUiHandlerPtr = std::unique_ptr<ShipStatusUiHandler>;

} // namespace pge
