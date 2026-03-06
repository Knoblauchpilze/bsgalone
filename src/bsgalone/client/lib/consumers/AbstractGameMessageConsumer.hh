
#pragma once

#include "AbstractMessageConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgalone::client {

class AbstractGameMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  AbstractGameMessageConsumer(const std::string &name, const core::MessageType relevantMessageType);
  ~AbstractGameMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  protected:
  virtual void onMessageReceivedInternal(const core::IMessage &message) = 0;

  private:
  bool m_relevantLoadingTransitionDetected{false};

  void handleLoadingStarted(const core::LoadingStartedMessage &message);
  void handleLoadingFinished(const core::LoadingFinishedMessage &message);
  void handleRelevantMessage(const core::IMessage &message);
};

} // namespace bsgalone::client
