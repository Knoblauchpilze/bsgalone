

#pragma once

#include "IMessage.hh"

namespace bsgalone::core {

class ConnectionMessage : public IMessage
{
  public:
  ConnectionMessage();
  ~ConnectionMessage() override = default;

  auto clone() const -> IMessagePtr override;
};

} // namespace bsgalone::core
