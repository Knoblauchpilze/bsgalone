
#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class AbstractSystemMessage : public AbstractMessage
{
  public:
  ~AbstractSystemMessage() override = default;

  auto getSystemDbId() const -> Uuid;

  protected:
  AbstractSystemMessage(const MessageType &type, const Uuid systemDbId);

  Uuid m_systemDbId{};
};

} // namespace bsgo
