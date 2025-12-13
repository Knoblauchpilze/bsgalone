
#pragma once

#include "AbstractMessageConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace pge {

class AbstractGameMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  AbstractGameMessageConsumer(const std::string &name, const bsgo::MessageType relevantMessageType);
  ~AbstractGameMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  protected:
  virtual void onMessageReceivedInternal(const bsgo::IMessage &message) = 0;

  private:
  bool m_relevantLoadingTransitionDetected{false};

  void handleLoadingStarted(const bsgo::LoadingStartedMessage &message);
  void handleLoadingFinished(const bsgo::LoadingFinishedMessage &message);
  void handleRelevantMessage(const bsgo::IMessage &message);
};

} // namespace pge
