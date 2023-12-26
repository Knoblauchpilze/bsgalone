
#pragma once

#include "IMessage.hh"

namespace bsgo {

class AbstractMessage : public IMessage
{
  public:
  AbstractMessage(const MessageType &type);
  ~AbstractMessage() override = default;

  auto type() const -> MessageType override;

  private:
  MessageType m_messageType{};
};

} // namespace bsgo
