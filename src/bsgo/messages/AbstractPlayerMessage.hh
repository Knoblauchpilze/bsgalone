
#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class AbstractPlayerMessage : public AbstractMessage
{
  public:
  ~AbstractPlayerMessage() override = default;

  auto getPlayerDbId() const -> bsgo::Uuid;

  protected:
  AbstractPlayerMessage(const bsgalone::core::MessageType &type, const bsgo::Uuid playerDbId);

  bsgo::Uuid m_playerDbId{};
};

} // namespace bsgalone::core
