
#pragma once

#include "IMessage.hh"

namespace bsgalone::core {

class AbstractMessage : public IMessage
{
  public:
  AbstractMessage(const bsgo::MessageType &type);
  ~AbstractMessage() override = default;

  auto type() const -> bsgo::MessageType override;

  protected:
  bsgo::MessageType m_messageType{};
};

} // namespace bsgalone::core
