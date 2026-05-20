
#pragma once

#include "IDataStore.hh"
#include "IMessageListener.hh"
#include "IUiEventQueue.hh"

namespace bsgalone::client {

class SystemDataMessageConsumer : public core::IMessageListener
{
  public:
  SystemDataMessageConsumer(IDataStoreShPtr store);
  ~SystemDataMessageConsumer() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &event) override;

  private:
  IDataStoreShPtr m_store{};
};

} // namespace bsgalone::client
