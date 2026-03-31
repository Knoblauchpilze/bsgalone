
#pragma once

#include "IDataStore.hh"
#include "IMessageListener.hh"

namespace bsgalone::client {

class LoginMessageConsumer : public core::IMessageListener
{
  public:
  LoginMessageConsumer(IDataStoreShPtr store);
  ~LoginMessageConsumer() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &event) override;

  private:
  IDataStoreShPtr m_store{};
};

} // namespace bsgalone::client
