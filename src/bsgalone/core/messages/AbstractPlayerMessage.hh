
#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class AbstractPlayerMessage : public AbstractMessage
{
  public:
  ~AbstractPlayerMessage() override = default;

  auto getPlayerDbId() const -> bsgo::Uuid;
  auto getSystemDbId() const -> bsgo::Uuid;

  protected:
  AbstractPlayerMessage(const MessageType &type,
                        const bsgo::Uuid playerDbId,
                        const bsgo::Uuid systemDbId);

  bsgo::Uuid m_playerDbId{};
  bsgo::Uuid m_systemDbId{};
};

} // namespace bsgalone::core
