
#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class AbstractPlayerMessage : public AbstractMessage
{
  public:
  ~AbstractPlayerMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getSystemDbId() const -> Uuid;

  protected:
  AbstractPlayerMessage(const MessageType &type, const Uuid playerDbId, const Uuid systemDbId);

  Uuid m_playerDbId{};
  Uuid m_systemDbId{};
};

} // namespace bsgalone::core
