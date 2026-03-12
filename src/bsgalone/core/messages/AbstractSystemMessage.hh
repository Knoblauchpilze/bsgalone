
#pragma once

#include "IMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class AbstractSystemMessage : public IMessage
{
  public:
  ~AbstractSystemMessage() override = default;

  auto getSystemDbId() const -> Uuid;

  protected:
  AbstractSystemMessage(const MessageType &type, const Uuid systemDbId);

  Uuid m_systemDbId{};
};

} // namespace bsgalone::core
