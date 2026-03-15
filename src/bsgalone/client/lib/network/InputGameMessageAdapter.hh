
#pragma once

#include "AbstractMessageListener.hh"

namespace bsgalone::client {

class InputGameMessageAdapter : public core::AbstractMessageListener
{
  public:
  InputGameMessageAdapter();
  ~InputGameMessageAdapter() override = default;

  void onEventReceived(const core::IMessage &event) override;

  private:
};

} // namespace bsgalone::client
