
#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class AbstractSystemMessage : public AbstractMessage
{
  public:
  ~AbstractSystemMessage() override = default;

  auto getSystemDbId() const -> bsgo::Uuid;

  protected:
  AbstractSystemMessage(const MessageType &type, const bsgo::Uuid systemDbId);

  bsgo::Uuid m_systemDbId{};
};

} // namespace bsgalone::core
